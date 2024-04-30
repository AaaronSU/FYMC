#define _DEFAULT_SOURCE

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <endian.h>

// #include "../parse/parse_code/parse_code.h"
// #include "../parse/parse_data/parse_data.h"
// #include "../parse/parse.h"
#include "../tools/tools.h"
#include "../tools/types.h"


i64 get_max(char* mask)
{
  i64 taille = strlen(mask);
  i64 return_value = 0;
  for (i64 i = 2; i < taille; ++i)
  {
    if (mask[i] == '1')
    {
      return_value = (taille - i);
    }
  }
  return return_value;
}


i64 retreive_value(char* mask)
{
  i64 taille = strlen(mask);
  i64 return_value = 0;
  for (i64 i = 2; i < taille; ++i)
  {
    if (mask[i] == '1')
    {
      return_value += 1 << (taille - i);
    }
  }
  return taille;
}


// Checks if ligne[0] is in address_array
bool its_an_alias(char* ligne, char** address_array, u64* i)
{
  for (*i=1; *i <= atoi(address_array[0]); *i = *i + 1)
  // for (u64 i = 1; address_array[i] != NULL; ++i)
  {
    // Alias gotta be first in line
    if (strcmp(ligne, address_array[*i]) == 0)
    {
      return true;
    }
  }
  return false;
}


i32 get_op_position(char* to_find, char*** op_name_list, i32 size)
{
  i32 position = -1;
  for (i32 i = 0; i < size; ++i)
  {
    if (strcasecmp(to_find, op_name_list[i][0]) == 0)
      {
        position = i;
        break;
      }
  }
  return position;
}



// Returns the 32bits sequence we need to write for ligne
void code_to_number(char*** tokens_list,   i32* tokens_sizes, i32 tokens_size,
                    i32     code_start,    i32 indice,
                    char**  address_array, u64* address_indices,
                    char*** op_name_list,  i32 op_list_size,
                    i32*    to_write,      i64* immediate_value,
                    char*** register_list, i32* reg_sizes, i32 reg_size,
                    bool*   imm, bool* vec,
                    i64*    ascii_address, char** ascii_written, bool* thread_on)
{
  // We'll write 32 bits => 4 bytes => 1 i32
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
    The function calculates the i32 corresponding to the instruction (see above)

   mov and parallelism are handled in their own way as they require extra work

   */


  i32 position   = get_op_position(tokens_list[indice][0],
                                   op_name_list, op_list_size);

  if (position == 91 || position == 92)
  {
    return;
  }

  if (position == 93)
  {
    *thread_on = true;
    return;
  }

  i32 imm_indice = 0;
  *to_write      += position << 24;

  // If mov
  if (position == 12)
  {
    if (tokens_list[indice][0][0]    == 'V'
        || tokens_list[indice][0][0] == 'T'
        || tokens_list[indice][0][0] == 'G')
    {
      *to_write += 1 << 19;
    }
    for (u64 i = 1; i < tokens_sizes[indice]; ++i)
    {
      if (tokens_list[indice][i][0]    == 'S'
          || tokens_list[indice][i][0] == 'T')
      {
        // 19 - 2 * 1 = 17 ; 19 - 2 * 2 = 15
        *to_write += 1 << (19 - 2 * i);
      }
      else if (tokens_list[indice][i][0]    == 'F'
               || tokens_list[indice][i][0] == 'G')
      {
        *to_write += 2 << (19 - 2 * i);
      }
    }
  }

  // Looking for registers
  //TODO addresses
  for (i32 i = 1; i < tokens_sizes[indice]; ++i)
  {
    // If it's a register or an alias
    if (isalpha(tokens_list[indice][i][0]))
    {
      // If it's an alias
//      u64 j = 1;
//      if (its_an_alias(tokens_list[indice][i], address_array, &j))
//      {
//        immediate_value[0] = address_indices[j];
//        *imm = true;
//        continue;
//      }


      // If it's a register
      char* str_tmp = calloc(tokens_sizes[indice], sizeof(char));
      if (str_tmp == NULL)
      {
        perror("calloc in code_to_number");
        return;
      }
      strncpy(str_tmp, tokens_list[indice][i]+1, tokens_sizes[indice]);
      *to_write += atoi(str_tmp) << (15 - 5 * i);
      free(str_tmp);

      for (u64 k = 0; k < reg_size; ++k)
      {
        if (tokens_list[indice][i][0] == register_list[k][0][0])
        {
          *to_write += i << (19);
        }
      }
    }

    // If it's parenthesis
    else if (tokens_list[indice][i][0]    == '('
             || tokens_list[indice][i][0] == ')')
    {
      continue;
    }

    // If it's brace
    else if (tokens_list[indice][i][0]    == '{'
             || tokens_list[indice][i][0] == '}')
    {
      continue;
    }

    // Handling immediate values
    else if (isdigit(tokens_list[indice][i][0]))
    {
      if (*imm == true)
      {
        *vec = true;
      }
      else
      {
        *imm = true;
      }
      immediate_value[imm_indice] = atoi(tokens_list[indice][i]);
      // *to_write += 1 << (15);
      *to_write += immediate_value[imm_indice++] << (15);

    }

    // Handling variables
    else if (tokens_list[indice][i][0] == '$')
    {
      for (u64 j = 1; j < code_start; ++j)
      {
        if (strcmp(tokens_list[j][1], tokens_list[indice][i]+1) == 0)
        {
          immediate_value[imm_indice++] = atoi(tokens_list[j][2]);
          break;
        }
      }
      // *to_write += 1 << (15);
      if (*imm == true)
      {
        *vec = true;
      }
      else
      {
        *imm = true;
      }
    }

    // Handling pointers and ascii
    else if (tokens_list[indice][i][0] == '@')
    {
      // look into data array and write value
      for (u64 j = 1; j < code_start; ++j)
      {
        if (strcmp(tokens_list[j][1], tokens_list[indice][i]+1) == 0)
        {
          immediate_value[imm_indice++] = atoi(tokens_list[j][2]);
          break;
        }
        if (ascii_written[i] != NULL
            && strcmp(ascii_written[i], tokens_list[indice][i]+1) == 0)
        {
          immediate_value[imm_indice++] = ascii_address[i];
          break;
        }
      }
      // *to_write += 1 << (15);
      if (*imm == true)
      {
        *vec = true;
      }
      else
      {
        *imm = true;
      }
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


i64 assemble_code(char***   tokens_list,
                  i32 const data_start,    i32 const code_start,
                  i32 const tokens_size,   i32*      tokens_sizes,
                  char***   op_name_list,  i32       op_list_size,
                  char***   register_list, i32*      reg_sizes, i32 reg_size,
                  char**    address_array, i32*      code_to_write,
                  i64*      imm_to_write,  i64*      imm_indice,
                  i64*      ascii_address, char**    ascii_written,
                  u64*      taille_code,   i32       address_size,
                  i32*      there_is_something_to_write,
                  i64*      thread_array,  i64* thread_mask, i64* thread_number,
                  i64*      thread_max)
{
  // We need to know on which line they are
  // address_array[i] <=> address_indices[i]
  // (Do note that address_indices[0] = 0)
  u64* address_indices = calloc(address_size, sizeof(u64));
  i32 j                = 1;
  i32 l                = 0;
  i64 taille           = 0;

  for (u64 i = code_start + 1; i < tokens_size; ++i)
  {
    // print_tokens_line(code_array[i]);
    // if (its_an_alias(code_array[i], address_array))
    if (tokens_list[i][0][strlen(tokens_list[i][0]) - 1] == 58)
    {
      address_indices[j++] = taille + 1; // ?
      continue;
    }

    i32  instr_to_write   = 0;
    i64* immediate_value  = calloc(8, sizeof(i64));
    bool imm              = false;
    bool vec              = false;
    bool thread_on        = false;

    code_to_number(tokens_list,     tokens_sizes, tokens_size,
                   code_start,      i,
                   address_array,   address_indices,
                   op_name_list,    op_list_size,
                   &instr_to_write, immediate_value,
                   register_list,   reg_sizes, reg_size,
                   &imm, &vec,
                   ascii_address,   ascii_written, &thread_on);
//    printf("instr: %s ; imm: %d ; vec: %d\n", tokens_list[i][0], imm, vec);


    if (thread_on == true)
    {
      i64 tmp_number           = *thread_number + 1;
      *thread_number           = tmp_number;
      thread_array[tmp_number] = *taille_code;
      thread_mask[tmp_number]  = retreive_value(tokens_list[i][1]);
      i64 max                  = get_max(tokens_list[i][1]);
      if (max > *thread_max)
      {
        *thread_max = max;
      }
      continue;
    }

    if (imm == true)
    {
      if (vec == false)
      {
        instr_to_write += immediate_value[0] << 15;
      }
      else
      {
        *there_is_something_to_write = true;
        //TODO replace 8 by vec_size (define?)
        for (u64 j = 0; j < 8; ++j)
        {
          imm_to_write[l] = htobe64(immediate_value[j]);
          imm_indice[l]   = *taille_code;
          ++l;
          taille += sizeof(i64);  // Immediate value is 64 bits -> 8 bytes
          // taille += sizeof(i64);
        }
      }
    }
    code_to_write[*taille_code] = htobe32(instr_to_write);
    *taille_code                = *taille_code + 1;
    taille                      += sizeof(i32);
    // 1 i32 = 4 bytes
    // taille += sizeof(i32);

    free(immediate_value);
  }

  free(address_indices);
  return taille;
}


i64 assemble_data(char*** tokens_list,   i32 const code_start,
                  char**  data_to_write, i64*      nb_data_to_write,
                  i64*    ascii_address, char**    ascii_written)
{
  i32 indice      = 0;
  i64 retour = 0;
  for (u64 i = 0; i < code_start; ++i)
  {
    if (strcmp(tokens_list[i][0], "ascii") == 0)
    {
      // taille is lenght of string including white space
      i32 taille = strlen(tokens_list[i][2]) + 1;
      data_to_write[indice] = malloc(taille * sizeof(char));
      ascii_written[indice] = malloc(taille * sizeof(char));
      ascii_address[indice] = retour;

      memcpy(data_to_write[indice],   tokens_list[i][2], taille);
      strcpy(ascii_written[indice++], tokens_list[i][1]);  // Retaining who we wrote
      retour += taille;
      *nb_data_to_write += 1;
    }
  }
  return retour;
}


void write_header(FILE* fp, i64 size_data, i64 size_code, i64 thread_number, i64 max_thread)
{
  // TODO: check those addresses (might lack a +1 or something) and magic_number
  i64 magic_number    = htobe64(0x4152434859302E30); // Check that
  i64 size_header     = htobe64(72);
  i64 address_threads = htobe64(size_header);
  i64 size_threads    = htobe64(thread_number);
  i64 address_data    = htobe64(address_threads + size_threads);
  i64 address_code    = htobe64(address_data + size_data);
  i64 size_total      = htobe64(address_code + size_code);
  //TODO add parallel data in header
  i64 to_write[8]  = {magic_number, size_header,
                      address_data, address_code,
                      max_thread, address_threads,
                      size_threads, size_total};
  fwrite(to_write, sizeof(i64), 8, fp);  // With 8 sizeof i64 (8 bytes)
  return;
}


void write_data(FILE* fp, char** data_to_write, i64* nb_data_to_write)
{
  for (u64 i = 0; i < *nb_data_to_write; ++i)
  {
//    printf("data: '%s'\n", data_to_write[i]);
    fwrite(data_to_write[i], sizeof(char), strlen(data_to_write[i]) + 1, fp);
  }
  return;
}


void write_code(FILE* fp,           i32* code_to_write,
                i64*  imm_to_write, i64* imm_indice, u64 k,
                i32   there_is_something_to_write)
{
  i32 j = 0;
  for (u64 i = 0; i < k; ++i)
  {
//    printf("instr: %d\n", code_to_write[i]);
    fwrite(code_to_write+i, sizeof(i32), 1, fp);

    // Write immediate values (e.g. movui)
    if (there_is_something_to_write == false)
    {
      continue;
    }
    while (i == imm_indice[j])
    {
     // printf("imm: %ld\n", imm_to_write[j]);
      fwrite(imm_to_write+j, sizeof(i64), 1, fp);
      ++j;
    }
  }
  return;
}


void write_threads(FILE* fp, i64* thread_array, i64* thread_masks, i64 thread_number)
{
  for (i64 i = 0; i < thread_number; ++i)
  {
//    printf("data: '%s'\n", data_to_write[i]);
    fwrite(htobe64(thread_array[i]), sizeof(i32), 1, fp);
    fwrite(htobe64(thread_masks[i]), sizeof(i32), 1, fp);
  }
  return;
}


void write_stuff(char*     path,
                 char***   tokens_list,   i32* tokens_list_sizes, i32 nb_tokens,
                 i32 const data_start,    i32 const code_start,
                 char***   op_name_list,  i32* op_sizes,          i32 op_size,
                 char***   register_list, i32* register_sizes,    i32 register_size,
                 char**    address_array, i32  address_size)
{
  // FILE *write_ptr = fopen("assembled", "wb");
  // i32* buf[10];
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

  FILE*       file;
  struct stat st;

  if (stat(path, &st) == -1)
  {
    perror("stat in write_stuff");
    return;
  }
  if (!S_ISREG(st.st_mode))
  {
    fprintf(stderr, "%s is a directory\n", path);
    return;
  }

  file = fopen(path, "w");
  if(file == NULL)
  {
    fprintf(stderr, "Could not open %s\n", path);
    perror("fopen in write_stuff");
    return;
  }


  // Size will be set to maximum size for the sake of simplicity
  char** data_to_write = calloc(code_start,   sizeof(char*));
  char** ascii_written = calloc(code_start,   sizeof(char*));
  i64*   ascii_address = calloc(address_size, sizeof(i64));
  i32*   code_to_write = calloc(nb_tokens - data_start, sizeof(i32));
  i64*   imm_to_write  = calloc(nb_tokens - data_start, sizeof(i64));
  i64*   imm_indice    = calloc(nb_tokens - data_start, sizeof(i64));
  i64*   thread_array  = calloc(nb_tokens - code_start, sizeof(i64));
  i64*   thread_masks  = calloc(nb_tokens - code_start, sizeof(i64));
  i64    thread_number = 0;
  i64    thread_max    = 0;

  // En fait go avoir
  /*
   liste d'entiers
   liste de long
   liste d'indices
   */

  i64 nb_data_to_write            = 0;
  u64 taille_code                 = 0;
  i32 there_is_something_to_write = false;

  i64 size_data = assemble_data(tokens_list, code_start, data_to_write,
                                &nb_data_to_write,
                                ascii_address, ascii_written);

  i64 size_code = assemble_code(tokens_list,   data_start, code_start,
                                nb_tokens,     tokens_list_sizes,
                                op_name_list,  op_size,
                                register_list, register_sizes,
                                register_size,
                                address_array, code_to_write,
                                imm_to_write,  imm_indice,
                                ascii_address, ascii_written,
                                &taille_code,  address_size,
                                &there_is_something_to_write,
                                thread_array, thread_masks, &thread_number,
                                &thread_max);

  write_header (file, size_data,     size_code,    thread_number, thread_max);
  write_threads(file, thread_array,  thread_masks, thread_number);
  write_data   (file, data_to_write, &nb_data_to_write);
  write_code   (file, code_to_write,
                imm_to_write, imm_indice, taille_code,
                there_is_something_to_write);

  /*
   for code in fwrite
     write
     if indice == imm_indice[k]
       write imm_to_write[k++]
    NB: imm_indice may contain the same indice multiple times (for vectors)
   */

  free_char2(data_to_write, code_start);
  free_char2(ascii_written, code_start);
  free(code_to_write);
  free(ascii_address);
  free(imm_to_write);
  free(imm_indice);
  free(thread_array);
  free(thread_masks);

  // Header -> Il faut "juste" calculer les tailles des autres parties en gros
  // cf doc pour détails

  // Datas -> On écrit que les ascii, le reste on remplace dans le code

  // Code -> opcode -> nombre -> écrit tel quel (en binaire)
        // registres -> écrits à côté, en respectant le format

  (void)fclose(file);

  // printf("Success\n");

  return;
}
