#include "parse_code.h"
// #include "../../tools/tools.h"
#include "../../tools/types.h"
// #include "../parse_data/parse_data.h"
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/// Returns the indice of the opcode provided
/// If the opcode is not found, returns -1
i32 good_opcode(char* in, char*** opcodes, i32 len_op)
{
  i32 return_value = -1;

  for (i32 i = 0; i < len_op; ++i)
  {
    if (strcasecmp(in, opcodes[i][0]) == 0)
    {
      return_value = i;
      break;
    }
  }

  return return_value;
}


/// Checks if in is a correct register
bool good_register(char* in, char*** registers, i32 len_registers, char letter)
{
  bool return_value;

  // Checks if number is correct
  u64 len = strlen(in);
  for (u64 i = 1; i < len; ++i)
  {
    if (isdigit(in[i]) == 0)
    {
      return_value = false;
      goto END_good_register;
    }
  }

  i32 indice_register;
  for (indice_register = 0; indice_register < len_registers; ++indice_register)
  {
    // cf register_list file
    if (registers[indice_register][0][0] == letter)
    {
      goto NUMBER_good_register;
    }
  }
  return_value = false;
  goto END_good_register;

  NUMBER_good_register:
  if (atoi(in) >= atoi(registers[indice_register][1])
      && atoi(in) <= atoi(registers[indice_register][2]))
  {
    return_value = true;
  }
  else
  {
    return_value = false;
  }

  END_good_register:
  return return_value;
}


/// Checks if str is in list
///   if remove_label_sign is true, last element of str is omitted
///     and -1 will be returned if an error occured
i32 label_present(char* str, char** list, i32 list_size,
                  bool remove_label_sign)
{
  i32 value;
  u64 taille;
  char* tmp;
  if (remove_label_sign == true)
  {
    taille = strlen(str);
    tmp    = calloc(taille, sizeof(char));
    if (tmp == NULL)
    {
      perror("calloc in label_present");
      value = -1;
      goto END_label_present;
    }
    memcpy(tmp, str, taille - 1);
    for (i32 i = 0; i < list_size; ++i)
    {
      if (strcmp(tmp, list[i]) == 0)
      {
        value = true;
        free(tmp);
        goto END_label_present;
      }
    }
    free(tmp);
  }
  else
  {
    for (i32 i = 0; i < list_size; ++i)
    {
      if (strcmp(str, list[i]) == 0)
      {
        value = true;
        goto END_label_present;
      }
    }
  }
  value = false;

  END_label_present:
  return value;
}
