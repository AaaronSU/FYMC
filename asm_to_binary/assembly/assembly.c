#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "../parse/parse_code/parse_code.h"
#include "../parse/parse_data/parse_data.h"
#include "../parse/parse.h"
#include "../tools/tools.h"



// Checks if ligne[0] is in address_array
bool its_an_alias(char** ligne, char** address_array)
{
  for (size_t i = 1; i <= atoi(address_array[0]); ++i)
  {
    // Alias gotta be on first line
    if (strcmp(ligne[0], address_array[i]) == 0)
      return TRUE;
  }
  return FALSE;
}


// Returns the 32bits sequence we need to write for ligne
// WARNING variable size due to immediate values, will be adapted very soon
void code_to_number(char** ligne, char*** data_array,
                    char** address_array, size_t* address_indices, char*** op_name_list,
                    int* to_write)
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

  int position = detect_op_code(ligne[0], op_name_list);
  *to_write += position<<24;
  print_tokens_line(ligne);

  // Looking for registers
  // WARNING: This part will change with parenthesis
  for (int i = 1; i <= 3; ++i)
  {
    if (ligne[i] == NULL)
      break;
    // If it's a register
    if (isalpha(ligne[i][0]))
    {
      char* str_tmp = calloc(strlen(ligne[i]), sizeof(char));
      strncpy(str_tmp, ligne[i]+1, strlen(ligne[i]));
      *to_write += atoi(str_tmp) << (15 - 5 * i);
      free(str_tmp);
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





void assemble_code(char*** data_array, char*** code_array,
                long long int* const nb_data, long long int* const nb_code,
                char*** op_name_list, char*** register_list, char** address_array, int* code_to_write)
{
  // We need to know on which line they are
  // address_array[i] <=> address_indices[i]
  // (Do note that address_indices[0] = 0)
  size_t* address_indices = calloc(atoi(address_array[0]), sizeof(size_t));
  int j = 1;

  for (size_t i = 0; i < *nb_code; ++i)
  {
    if (its_an_alias(code_array[i], address_array))
    {
      address_indices[j] = i;
      continue;
    }

    int to_write = 0;

    code_to_number(code_array[i], data_array, address_array, address_indices, op_name_list, &to_write);

    code_to_write[i] = to_write;

  }

  free(address_indices);
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

  FILE *file = fopen(path, "wb");

  // Si impossible d'ouvrir le fichier
  if (!file)
  {
    perror("fopen");
    (void)fclose(file);
    return; // NOTE: crash here?
  }


  // Dumb and may not even be enough
  char* my_big_calloc = calloc(atoi(address_array[0]) * *nb_data * *nb_code, sizeof(char));

  // WARNING variable size due to immediate values, will be adapted very soon
  // Size will be set to maximum size for the sake of simplicity
  int* code_to_write = calloc(*nb_code, sizeof(int));

  assemble_code(data_array, code_array, nb_data, nb_code,
                op_name_list, register_list, address_array, code_to_write);

  free(code_to_write);
  free(my_big_calloc);

  // Header -> Il faut "juste" calculer les tailles des autres parties en gros
  // cf doc pour détails

  // Datas -> On écrit que les ascii, le reste on remplace dans le code

  // Code -> opcode -> nombre -> écrit tel quel (en binaire)
        // registres -> écrits à côté, en respectant le format
        // cf messages épinglés (Discord)

  (void)fclose(file);
  return;
}
