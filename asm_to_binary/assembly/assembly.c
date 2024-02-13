#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "../parse/parse_code/parse_code.h"
#include "../parse/parse_data/parse_data.h"
#include "../parse/parse.h"
#include "../tools/tools.h"



// Checks if ligne[0] is in address_array
bool its_an_alias(char* ligne, char** address_array, size_t* i)
{
  for (*i=1; *i <= atoi(address_array[0]); *i = *i + 1)
  // for (size_t i = 1; address_array[i] != NULL; ++i)
  {
    // Alias gotta be first in line
    if (strcmp(ligne, address_array[*i]) == 0)
      return TRUE;
  }
  return FALSE;
}


// Returns the 32bits sequence we need to write for ligne
void code_to_number(char** ligne, char*** data_array,
                    char** address_array, size_t* address_indices, char*** op_name_list,
                    int* to_write, long* immediate_value, char*** register_list, bool* imm, bool* vec,
                    long int* ascii_address, char** ascii_written)
{
  // We'll write 32 bits => 4 bytes => 1 int
  // 8 bits opcode
  // 9 bits "unused"
  // 5 destination
  // 5 src1
  // 5 src2
  //
  // 01 02 03 04 05 06 07 08  01 02 03 04 05 06 07 08 09  01 02 03 04 05  01 02 03 04 05  01 02 03 04 05
  // 31 30 29 28 27 26 25 24  23 22 21 20 19 18 17 16 15  14 13 12 11 10  09 08 07 06 05  04 03 02 01 00
  //
  // 2^24 ~ 2^32-1
  // opcode 0 -> 0
  // opcode 1 -> 2^24
  // opcode 2 -> 2^25
  // opcode << 24           unused will be calculated  dest << 10         src1 << 5       src2 << 0

  /*
   General idea :
    The function calculates the int corresponding to the instruction (see above)

   mov is handled in it's own way as it requires extra work

   */


  int position = detect_op_code(ligne[0], op_name_list);
  *to_write += position << 24;

  // If mov
  if (position == 12)
  {
    if (ligne[0][0] == 'V' || ligne[0][0] == 'T' || ligne[0][0] == 'G')
      *to_write += 1 << 19;
    for (size_t i = 1; i < 3; ++i)
    {
      if (ligne[i][0] == 'S' || ligne[i][0] == 'T')
      {
        // 19 - 2 * 1 = 17 ; 19 - 2 * 2 = 15
        *to_write += 1 << (19 - 2 * i);
      }
      else if (ligne[i][0] == 'F' || ligne[i][0] == 'G')
      {
        *to_write += 2 << (19 - 2 * i);
      }
    }
  }

  // Looking for registers
  //TODO addresses
  for (int i = 1; i <= 3; ++i)
  {
    if (ligne[i] == NULL)
      break;
    // If it's a register or an alias
    if (isalpha(ligne[i][0]))
    {
      // If it's an alias
      size_t j = 1;
      if (its_an_alias(ligne[i], address_array, &j))
      {
        immediate_value[0] = address_indices[j];
        *imm = TRUE;
        continue;
      }


      // If it's a register
      char* str_tmp = calloc(strlen(ligne[i]), sizeof(char));
      strncpy(str_tmp, ligne[i]+1, strlen(ligne[i]));
      *to_write += atoi(str_tmp) << (15 - 5 * i);
      free(str_tmp);

      for (size_t k = 0; register_list[k] != NULL; ++k)
      {
        if (ligne[i][0] == register_list[k][0][0])
          *to_write += i << (19);
      }
    }

    // If it's parenthesis
    else if (ligne[i][0] == '(')
    {
      // Tokenizing
      char* char_tmp = calloc(strlen(ligne[i]), sizeof(char));
      strncpy(char_tmp, ligne[i]+1, strlen(ligne[i])-2);
      strcat(char_tmp, "\0");
      char** token_thing = retreive_token(char_tmp, ',');
      free(char_tmp);

      // Handling ()
      for (size_t j = 0; j < 2; ++j)
      {
        if (token_thing[j] == NULL)
          break;  // Not supposed to happen?

        char* str_tmp = calloc(strlen(token_thing[j]), sizeof(char));
        strncpy(str_tmp, token_thing[j]+1, strlen(token_thing[j]));
        *to_write += atoi(str_tmp) << (15 - 5 * i);
        free(str_tmp);
        ++i;
      }

      // Handling immediate value inside ()
      // immediate here is an offset and should be writable on 9 bits
      if (token_thing[3] != NULL)
      {
        //NOTE We don't actually need to use immediate_value array
        immediate_value[0] = atoi(token_thing[3]);
        *to_write += immediate_value[0] << 15;
        // *imm = TRUE;
      }
      free(token_thing);
      break;
    }

    // If it's brace
    else if (ligne[i][0] == '{')
    {
      // Tokenizing
      char* char_tmp = calloc(strlen(ligne[i]), sizeof(char));
      strncpy(char_tmp, ligne[i]+1, strlen(ligne[i])-2);
      strcat(char_tmp, "\0");
      char** token_thing = retreive_token(char_tmp, ',');
      free(char_tmp);

      // Tell mama function we have vectorial immediate to write
      *imm = TRUE;
      *vec = TRUE;

      // Handling {}
      for (size_t j = 0; j < 8; ++j)
      {
        if (token_thing[j] == NULL)
          break;  // Not supposed to happen?

        immediate_value[j] = atoi(token_thing[i]);
        ++i;
      }
      free(token_thing);
      break;
    }

    // Handling immediate values
    else if (isdigit(ligne[i][0]))
    {
      //NOTE We don't actually need to use immediate_value array
      immediate_value[0] = atoi(ligne[i]);
      // *to_write += 1 << (15);
      *to_write += immediate_value[0] << (15);
      // *imm = TRUE;
    }

    // Handling variables
    else if (ligne[i][0] == '$')
    {
      for (size_t i = 0; data_array[i] != NULL; ++i)
      {
        if (strcmp(data_array[i][1], ligne[i]+1) == 0)
        {
          immediate_value[0] = atoi(data_array[i][2]);
          break;
        }
      }
      // *to_write += 1 << (15);
      *imm = TRUE;
    }

    // Handling pointers and ascii
    else if (ligne[i][0] == '@')
    {
      // look into data array and write value
      for (size_t i = 0; data_array[i] != NULL; ++i)
      {
        if (strcmp(data_array[i][1], ligne[i]+1) == 0)
        {
          immediate_value[0] = atoi(data_array[i][2]);
          break;
        }
        if (ascii_written[i] != NULL
            && strcmp(ascii_written[i], ligne[i]+1) == 0)
        {
          immediate_value[0] = ascii_address[i];
          break;
        }
      }
      // *to_write += 1 << (15);
      *imm = TRUE;
    }
  }

  // Récupère la position de l'opcode
  //    -> Devient le début de ce qu'on écrit
  //    -> On définit les registres
  //    -> On regarde et gère si valeurs immédiates, variables etc
  //    -> On gère les bits "inutilisés"
  // -> Retour dans un buffer fourni en entrée
  return;
}





long int assemble_code(char*** data_array, char*** code_array,
                long int const size_data, long long int* const nb_code,
                char*** op_name_list, char*** register_list, char** address_array, int* code_to_write,
                  long int* imm_to_write, long long int* imm_indice,
                       long int* ascii_address, char** ascii_written, size_t* k)
{
  // We need to know on which line they are
  // address_array[i] <=> address_indices[i]
  // (Do note that address_indices[0] = 0)
  size_t* address_indices = calloc(atoi(address_array[0]), sizeof(size_t));
  int j = 1;
  // long long int k = 0;
  long long int l = 0;
  long int taille = 0;

  for (size_t i = 0; i < *nb_code; ++i)
  {
    // print_tokens_line(code_array[i]);
    // if (its_an_alias(code_array[i], address_array))
    if (detect_alias(code_array[i]))
    {
      address_indices[j++] = taille + size_data; // ?
      continue;
    }

    int instr_to_write = 0;
    long* immediate_value = calloc(8, sizeof(long));
    bool imm = FALSE;
    bool vec = FALSE;

    code_to_number(code_array[i], data_array, address_array,
                   address_indices, op_name_list, &instr_to_write,
                   immediate_value, register_list, &imm, &vec,
                   ascii_address, ascii_written);

    if (imm == TRUE)
    {
      int max = 1;
      if (vec == TRUE)
        {max = 8;}

      for (size_t j = 0; j < max; ++j)
      {
        imm_to_write[l] = immediate_value[j];
        imm_indice[l] = *k;
        ++l;
        taille += 8;  // Immediate value is 64 bits -> 8 bytes
        // taille += sizeof(long int);
      }
    }
    code_to_write[*k] = instr_to_write;
    *k = *k + 1;
    taille += 4;  // 1 int = 4 bytes (for Linux ; TODO replace with u32)
    // taille += sizeof(int);

    free(immediate_value);
  }

  free(address_indices);
  return taille;
}


long int assemble_data(char*** data_array, long long int* const nb_data,
                       char** data_to_write, long int* nb_data_to_write,
                       long int* ascii_address, char** ascii_written)
{
  int indice = 0;
  long int retour = 0;
  for (size_t i = 0; i < *nb_data; ++i)
  {
    if (strcmp(data_array[i][0], "ascii") == 0)
    {
      // taille is lenght of string including white space
      int taille = strlen(data_array[i][2]) + 1;
      data_to_write[indice] = malloc(taille * sizeof(char));
      ascii_written[indice] = malloc(taille * sizeof(char));
      ascii_address[indice] = retour;
      memcpy(data_to_write[indice], data_array[i][2], taille);
      strcpy(ascii_written[indice++], data_array[i][1]);  // Retaining who we wrote
      retour += taille;
      *nb_data_to_write += 1;
    }
  }

  // NOTE: I forgot what that was supposed to achieve
  // free(data_to_write[indice]);
  // data_to_write[indice] = NULL;
  // free(ascii_written[indice]);
  // ascii_written[indice] = NULL;
  // retour *= sizeof(char);
  // retour *= 8;  // A char is 8 bits ; 1 byte is 8 bits
  return retour;
}


void write_header(FILE* fp, long int size_data, long int size_code)
{
  // TODO: check those addresses (might lack a +1 or something) and magic_number
  long int magic_number = 4706898538568625712; // Check that
  long int size_header = 48;
  long int address_data = size_header;
  long int address_code = address_data + size_data;
  long int size_total = address_code + size_code;
  long int to_write[5] = {magic_number, size_header, address_data, address_code, size_total};
  fwrite(to_write, 8, 5, fp);  // With 8 sizeof long int (8 bytes)
  return;
}


void write_data(FILE* fp, char** data_to_write, long int* nb_data_to_write)
{
  for (size_t i = 0; i < *nb_data_to_write; ++i)
  {
    fwrite(data_to_write[i], 1, strlen(data_to_write[i]) + 1, fp);
  }
  return;
}


void write_code(FILE* fp, int* code_to_write,
                long int* imm_to_write, long long int* imm_indice, size_t k)
{
  int j = 0;
  for (size_t i = 0; i < k; ++i)
  {
    fwrite(&code_to_write[i], 4, 1, fp);

    // Write immediate values (e.g. movui)
    while (i == imm_indice[j])
    {
      fwrite(&imm_to_write[j++], 8, 1, fp);
    }
  }
  return;
}




void write_stuff(char* path, char*** data_array, char*** code_array,
                long long int* const nb_data, long long int* const nb_code,
                char*** op_name_list, char*** register_list, char** address_array)
{
  // FILE *write_ptr = fopen("assembled", "wb");
  // int* buf[10];
  //
  // // buf -> data; 10 -> sizeofdata; 1 -> number of data; write_ptr -> destination
  // fwrite(buf, 10, 1, write_ptr);
  // fclose(write_ptr);


  /*
   NOTE
   Pour les ascii :
    j'écris et je garde en mémoire (dans une liste à ajouter) où je suis
      on conserve l'endroit par rapport au début de la section

    Pour les adresses d'ascii : on utilise le truc au dessus

    Pour les alias :
      Si on en croise sans opcode -> garder où il est (indice)
        Quand on doit l'écrire -> garder son emplacement (ajouter liste pour ça)
      Si on en croise avec opcode -> garder indice où il est
        Écrire son adresse (liste précédemment créée)

   */

  FILE *file = fopen(path, "wb");

  // Si impossible d'ouvrir le fichier
  if (!file)
  {
    perror("fopen");
    (void)fclose(file);
    return; // NOTE: crash here?
  }


  // Size will be set to maximum size for the sake of simplicity
  int* code_to_write = calloc(*nb_code, sizeof(int));
  char** data_to_write = calloc(*nb_data, sizeof(char*));
  char** ascii_written = calloc(*nb_data, sizeof(char*));
  long int* ascii_address = calloc(*nb_data, sizeof(char));
  long int* imm_to_write = calloc(*nb_code, sizeof(long int));
  long long int* imm_indice = calloc(*nb_code, sizeof(int));

  // En fait go avoir
  /*
   liste d'entiers
   liste de long
   liste d'indices
   */

  long int nb_data_to_write = 0;
  size_t taille_code = 0;

  long int size_data = assemble_data(data_array, nb_data, data_to_write,
                                     &nb_data_to_write, ascii_address, ascii_written);

  long int size_code = assemble_code(data_array, code_array, size_data, nb_code,
                                      op_name_list, register_list, address_array,
                                      code_to_write, imm_to_write, imm_indice,
                                      ascii_address, ascii_written, &taille_code);


  write_header(file, size_data, size_code);
  write_data(file, data_to_write, &nb_data_to_write);
  write_code(file, code_to_write, imm_to_write, imm_indice, taille_code);

  /*
   for code in fwrite
     write
     if indice == imm_indice[k]
       write imm_to_write[k++]
    NB: imm_indice may contain the same indice multiple times (for vectors)
   */

  free(code_to_write);
  free_char2(data_to_write, *nb_data);
  free_char2(ascii_written, *nb_data);
  free(ascii_address);
  free(imm_to_write);
  free(imm_indice);

  // Header -> Il faut "juste" calculer les tailles des autres parties en gros
  // cf doc pour détails

  // Datas -> On écrit que les ascii, le reste on remplace dans le code

  // Code -> opcode -> nombre -> écrit tel quel (en binaire)
        // registres -> écrits à côté, en respectant le format

  (void)fclose(file);

  printf("Success\n");

  return;
}
