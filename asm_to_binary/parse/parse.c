#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <sys/stat.h>

#include "parse_code/parse_code.h"
#include "parse_data/parse_data.h"
#include "parse.h"
// #include "../tools/tools.h"
#include "../tools/types.h"



//
bool alphanumeric(i32 str)
{
  // Check is ASCII code of character corresponds to a letter.
  return ((str >= 65 && str <= 90)
          || (str >= 97 && str <= 122)
          || (str >= 48 && str <=57));
}



//
i32 size_without_chara(const char* str, i32 const taille, i32 const chara)
{
  // Utiliser des pointeurs pour éviter les copies ? À voir
  i32 taille_out = taille;
  for (i32 i = 0; i < taille; ++i)
  {
    if (str[i] == chara)
    {
      taille_out -= sizeof(char);
    }
  }
  return taille_out;
}


// Detects if arg is in string AND before any ""
bool is_in_string(char* string, char* arg)
{
  i32 taille_arg    = strlen(arg);
  i32 taille_string = strlen(string);  // To not get out of range
  bool im_readin_it = false;
  u64 j = 0;
  if (!strstr(string, arg))
  {
    return false;
  }
  if (!strchr(string, '"'))
  {
    return true;
  }
  for (u64 i = 0; i < taille_string; ++i)
  {
    if (im_readin_it)
    {
      if (string[i] == arg[j])
      {
        ++j;
        if (j == taille_arg)
        {
          return true;
        }
      }
      im_readin_it = false;
      j = 0;
    }
    else if (string[i] == arg[j])
    {
      im_readin_it = true;
      ++j;
    }
    else if (string[i] == '"')
    {
      break;
    }
  }
  return false;
}


// Returns arg position in string AND if before any "", or -1 if not there
i32 where_in_string(char* string, char* arg)
{
  i32 taille_arg    = strlen(arg);
  i32 taille_string = strlen(string);  // To not get out of range
  bool im_readin_it = false;
  bool im_in_quote  = false;
  u64 j             = 0;
  for (u64 i = 0; i < taille_string; ++i)
  {
    // printf("'%c' '%c'\n", string[i], arg[j]);
    // Ignoring it if it's in quotes
    if (im_in_quote)
    {
      if (string[i] == '"')
      {
        im_in_quote = false;
      }
    }
    else if (im_readin_it)
    {
      if (string[i] == arg[j])
      {
        ++j;
        if (j == taille_arg)
        {
          return i - j;
        }
      }
      im_readin_it = false;
      j            = 0;
    }
    else if (string[i] == arg[j])
    {
      im_readin_it = true;
      ++j;
      if (j == taille_arg) // If arg is of size 1
      {
        return i - j;
      }
    }
    else if (string[i] == '"')
    {
      im_in_quote  = true;
      im_readin_it = false;
      j            = 0;
    }
  }
  return -1;
}


/// Check if in is a good instruction
/// Returns true (1) if it is valid, false (0) if it is not
///   The function may malloc in labels or requested_labels if required
///   If a malloc fails, -1 is returned and memory must be freed up to indice_labels and indice_req
i32 good_instruction(char*** in,         i32* len_in, i32 indice_in, char*** opcodes, i32 len_op,
                     i32     data_start, i32  code_start,
                     char*** registers,  i32* registers_sizes, i32 len_registers,
                     char**  labels,     i32* indice_labels,
                     char**  requested_labels, i32* indice_req, bool* parallel_on)
{
  i32 return_value;
  i32 opcode = good_opcode(in[indice_in][0], opcodes, len_op);
  if (opcode == -1)
  {
    // Handling addresses
    if (len_in[indice_in] == 1)
    {
      // If there's : at the end, and only at the end => good
      // Else => bad
      char* deux_points = strchr(in[indice_in][0], ':');
      if (deux_points != NULL)
      {
        i32 indice_deux_points = (i32)(deux_points - in[indice_in][0]);
        if (indice_deux_points == strlen(in[indice_in][0]) - 1)
        {
          // Adding it to potential labels ; should be checked outside the function
          return_value = true;
          i32 is_present;
          if (*indice_labels > 0)
          {
            is_present = label_present(in[indice_in][0],
                                       labels, *indice_labels, true);
            if (is_present == -1)
            {
              return_value = -1;
              goto END_good_instruction;
            }
            else if (is_present == true)
            {
              goto END_good_instruction;
            }
          }
          // Else
          i32 taille = strlen(in[indice_in][0]);
          // labels = realloc(labels, *indice_labels + 1);
          labels[*indice_labels] = calloc(taille, sizeof(char));
          if (labels[*indice_labels] == NULL)
          {
            perror("malloc in good_instruction");
            return_value = -1;
            goto END_good_instruction;
          }
          // Copies the label without the :
          //  terminating null byte is present thanks to calloc
          memcpy(labels[*indice_labels], in[indice_in][0], taille - 1);
          *indice_labels = *indice_labels + 1;
          goto END_good_instruction;
        }
        else
        {
          return_value = false;
          goto END_good_instruction;
        }

      }

      // TODO: Maybe remove that code duplication?
      else
      {
        return_value = false;
        goto END_good_instruction;
      }
    }
    else
    {
      return_value = false;
      goto END_good_instruction;
    }
  }

  if (opcode == 91)
  {
    if (*parallel_on == true)
    {
      fprintf(stderr, "Error: A parallel section is created before a previous one is closed.\n");
      return_value = false;
      goto END_good_instruction;
    }
    *parallel_on = true;
  }
  if (opcode == 92)
  {
    if (*parallel_on == false)
    {
      fprintf(stderr, "Error: A parallel section is closed before one is created.\n");
      return_value = false;
      goto END_good_instruction;
    }
    *parallel_on = false;
  }
  if (opcode > 92)
  {
    if (*parallel_on == false)
    {
      fprintf(stderr, "Error: A parallel instruction is called before a section is created.\n");
      return_value = false;
      goto END_good_instruction;
    }
  }

  // Checks if the number of argument is correct for the opcode
  i32 min = atoi(opcodes[opcode][1]);
  i32 max = atoi(opcodes[opcode][2]);
  if (len_in[indice_in] - 1 >= min && len_in[indice_in] - 1 <= max)
  {
    if (min == 0 && max == 0)
    {
      return_value = true;
      goto END_good_instruction;
    }
    else
    {
      i32 indice_opcode = 3;
      for (i32 i = 1; i < len_in[indice_in]; ++i)
      {
        if (opcodes[opcode][indice_opcode][0] == 'U'
            && toupper(in[indice_in][i][0])   == 'U')
        {
          if (good_register(in[indice_in][i], registers, len_registers, 'U')
              == true)
          {
            return_value = true;
            ++indice_opcode;
            continue;
          }
          else
          {
            return_value = false;
            goto END_good_instruction;
          }
        }
        else if (opcodes[opcode][indice_opcode][0] == 'S'
                 && toupper(in[indice_in][i][0])   == 'S')
        {
          if (good_register(in[indice_in][i], registers, len_registers, 'S')
              == true)
          {
            return_value = true;
            ++indice_opcode;
            continue;
          }
          else
          {
            return_value = false;
            goto END_good_instruction;
          }
        }
        else if (opcodes[opcode][indice_opcode][0] == 'F'
                 && toupper(in[indice_in][i][0])   == 'F')
        {
          if (good_register(in[indice_in][i], registers, len_registers, 'F')
              == true)
          {
            return_value = true;
            ++indice_opcode;
            continue;
          }
          else
          {
            return_value = false;
            goto END_good_instruction;
          }
        }
        else if (opcodes[opcode][indice_opcode][0] == 'V'
                 && toupper(in[indice_in][i][0])   == 'V')
        {
          if (good_register(in[indice_in][i], registers, len_registers, 'V')
              == true)
          {
            return_value = true;
            ++indice_opcode;
            continue;
          }
          else
          {
            return_value = false;
            goto END_good_instruction;
          }
        }
        else if (opcodes[opcode][indice_opcode][0] == 'T'
                 && toupper(in[indice_in][i][0])   == 'T')
        {
          if (good_register(in[indice_in][i], registers, len_registers, 'T')
              == true)
          {
            return_value = true;
            ++indice_opcode;
            continue;
          }
          else
          {
            return_value = false;
            goto END_good_instruction;
          }
        }
        else if (opcodes[opcode][indice_opcode][0] == 'G'
                 && toupper(in[indice_in][i][0])   == 'G')
        {
          if (good_register(in[indice_in][i], registers, len_registers, 'G')
              == true)
          {
            return_value = true;
            ++indice_opcode;
            continue;
          }
          else
          {
            return_value = false;
            goto END_good_instruction;
          }
        }
        else if (strcasecmp(opcodes[opcode][indice_opcode], "any") == 0)
        {
          if (good_register(in[indice_in][i], registers, len_registers, 'U')    == true
              || good_register(in[indice_in][i], registers, len_registers, 'S') == true
              || good_register(in[indice_in][i], registers, len_registers, 'F') == true
              || good_register(in[indice_in][i], registers, len_registers, 'V') == true
              || good_register(in[indice_in][i], registers, len_registers, 'T') == true
              || good_register(in[indice_in][i], registers, len_registers, 'G') == true)
          {
            return_value = true;
            ++indice_opcode;
            continue;
          }
          else
          {
            return_value = false;
            goto END_good_instruction;
          }
        }
        else if (strcasecmp(opcodes[opcode][indice_opcode], "off") == 0)
        {
          // Handling optionnal value
          if (len_in[indice_in] - 1 != max)
          {
            --i;
            ++indice_opcode;
            continue;
          }
          else
          {
            if (good_integer(in[indice_in][i], true) == true)
            {
              return_value = true;
              ++indice_opcode;
              continue;
            }
            else
            {
              i32 good = false;
              i32 incr = 0;
              if (in[indice_in][i][0] == '$')
              {
                incr++;
              }
              if (in[indice_in][i][0] == '$' || in[indice_in][i][0] == '@')
              {
                char* var_tmp = in[indice_in][i]+incr;
                for (i32 k = data_start + 1; k < code_start; ++k)
                {
                  // Finding the variable in the data section
                  if (strcmp(var_tmp, in[k][1]) == 0)
                  {
                    if (strcmp(in[k][0], "u64") == 0)
                    {
                      good = true;
                      break;
                    }
                  }
                }
              }
              if (good == true)
              {
                return_value = true;
                ++indice_opcode;
                continue;
              }
            }
            return_value = false;
            goto END_good_instruction;
          }
        }
        else if (strcasecmp(opcodes[opcode][indice_opcode], "ims")    == 0
                 || strcasecmp(opcodes[opcode][indice_opcode], "imt") == 0)
        {
          if (good_integer(in[indice_in][i], true) == true)
          {
            return_value = true;
            ++indice_opcode;
            continue;
          }
          else
          {
            i32 good = false;
            i32 incr = 0;
            if (in[indice_in][i][0] == '$' || in[indice_in][i][0] == '@')
            {
              char* var_tmp = in[indice_in][i]+1;
              char* var_tmp_at;
              if (in[indice_in][i][0] == '@')
              {
                var_tmp_at = in[indice_in][i];
              }
              else
              {
                var_tmp_at = var_tmp;
              }
              for (i32 k = data_start + 1; k < code_start; ++k)
              {
                // Finding the variable in the data section
                if (strcmp(var_tmp, in[k][1]) == 0
                    || strcmp(var_tmp, in[k][1]) == 0)
                {
                  if (strcmp(in[k][0], "u64") == 0
                      || strcmp(in[k][0], "ascii") == 0)
                  {
                    // printf("yay\n");
                    good = true;
                    break;
                  }
                }
              }
            }
            if (good == true)
            {
              return_value = true;
              ++indice_opcode;
              continue;
            }
          }
          return_value = false;
          goto END_good_instruction;
        }
        else if (strcasecmp(opcodes[opcode][indice_opcode], "imu")    == 0
                 || strcasecmp(opcodes[opcode][indice_opcode], "imv") == 0)
        {
          if (good_integer(in[indice_in][i], true) == true)
          {
            return_value = true;
            ++indice_opcode;
            continue;
          }
          else
          {
            i32 good = false;
            i32 incr = 0;
            if (in[indice_in][i][0] == '$' || in[indice_in][i][0] == '@')
            {
              char* var_tmp = in[indice_in][i]+1;
              char* var_tmp_at;
              if (in[indice_in][i][0] == '@')
              {
                var_tmp_at = in[indice_in][i];
              }
              else
              {
                var_tmp_at = var_tmp;
              }
              for (i32 k = data_start + 1; k < code_start; ++k)
              {
                // Finding the variable in the data section
                if (strcmp(var_tmp, in[k][1])    == 0
                    || strcmp(var_tmp, in[k][1]) == 0)
                {
                  if (strcmp(in[k][0], "u64")      == 0
                      || strcmp(in[k][0], "ascii") == 0)
                  {
                    good = true;
                    break;
                  }
                }
              }
            }
            if (good == true)
            {
              return_value = true;
              ++indice_opcode;
              continue;
            }
          }
          return_value = false;
          goto END_good_instruction;
        }
        else if (strcasecmp(opcodes[opcode][indice_opcode], "imf")     == 0
                 || (strcasecmp(opcodes[opcode][indice_opcode], "img") == 0))
        {
          if (good_float(in[indice_in][i]) == true)
          {
            return_value = true;
            ++indice_opcode;
            continue;
          }
          else
          {
            i32 good = false;
            i32 incr = 0;
            if (in[indice_in][i][0] == '$' || in[indice_in][i][0] == '@')
            {
              char* var_tmp = in[indice_in][i]+1;
              char* var_tmp_at;
              if (in[indice_in][i][0] == '@')
              {
                var_tmp_at = in[indice_in][i];
              }
              else
              {
                var_tmp_at = var_tmp;
              }
              for (i32 k = data_start + 1; k < code_start; ++k)
              {
                // Finding the variable in the data section
                if (strcmp(var_tmp, in[k][1]) == 0
                    || strcmp(var_tmp, in[k][1]) == 0)
                {
                  printf("good\n");
                  if (strcmp(in[k][0], "u64") == 0
                      || strcmp(in[k][0], "ascii") == 0)
                  {
                    good = true;
                    break;
                  }
                }
              }
            }
            if (good == true)
            {
              return_value = true;
              ++indice_opcode;
              continue;
            }
          }
          return_value = false;
          goto END_good_instruction;
        }
        else if (strcasecmp(opcodes[opcode][indice_opcode], "addr") == 0)
        {
          if (strchr(in[indice_in][i], ':') == NULL)
          {
            // Adding it to requested labels ; should be checked outside the function
            i32 is_present;
            if (*indice_req > 0)
            {
              is_present = label_present(in[indice_in][0],
                                         requested_labels, *indice_req, false);
              if (is_present == -1)
              {
                return_value = -1;
              }
              goto END_good_instruction;
            }
            // requested_labels = realloc(requested_labels, *indice_req + 1);
            requested_labels[*indice_req] = malloc(strlen(in[indice_in][i] + 1)
                                                   * sizeof(char));
            if (requested_labels[*indice_req] == NULL)
            {
              perror("malloc in good_instruction");
              return_value = -1;
              goto END_good_instruction;
            }
            strcpy(requested_labels[*indice_req], in[indice_in][i]);
            *indice_req = *indice_req + 1;
            ++indice_opcode;
            continue;
          }
        }
        else if (strcasecmp(opcodes[opcode][indice_opcode], "mask") == 0)
        {
          //TODO check if this works
          if (in[indice_in][i][0] == '0' && in[indice_in][i][1] == 'x')
          {
            i32 taille = strlen(in[indice_in][i]) - 1;
            char* tmp = malloc(taille * sizeof(char));
            memcpy(tmp, in[i]+2, taille);
            if (good_integer(tmp, false) == true)
            {
              free(tmp);
              return_value = true;
              ++indice_opcode;
              continue;
            }
            else
            {
              free(tmp);
              return_value = false;
              goto END_good_instruction;
            }
          }
        }
        else if ((opcodes[opcode][indice_opcode][0]   == '('
                  && in[indice_in][i][0] == '(')
                || (opcodes[opcode][indice_opcode][0] == ')'
                  && in[indice_in][i][0] == ')')
                || (opcodes[opcode][indice_opcode][0] == '{'
                  && in[indice_in][i][0] == '}')
                || (opcodes[opcode][indice_opcode][0] == '{'
                  && in[indice_in][i][0] == '}'))
        {
          return_value = true;
          ++indice_opcode;
          continue;
        }
        else
        {
          return_value = false;
          goto END_good_instruction;
        }
      }
    }
  }
  else
  {
    return_value = false;
  }

  END_good_instruction:
  return return_value;
}


/// Check every line from in to see if it matches assembly rules
/// in, sizes, data_start, code_start, opcodes, len_op, indice_labels and indice_req
///    should be initialized before calling the function
///
/// If return value is -1, an error occured upon reading file
/// If return value is -2, an error occured in tokenize but memory is clean
/// If return value is -3, an error occured in tokenize but memory hasn't been cleaned
i32 parse(char*** in,      i32* sizes, i32 len, i32* data_start, i32* code_start,
          char*** opcodes, i32  len_op,
          char**  labels,  i32* indice_labels,
          char**  requested_labels, i32* indice_req,
          char*** registers, i32* sizes_registers, i32 len_register)
{
  i32 return_value       = 0;
  i32 indice_line        = 0;
  i32 indice_within_line = 0;

  // char*** registers;
  // i32* sizes_registers;
  // i32 len_register;
  // FILE* f;
  //
  // i32 taille_fichier;
  // struct stat st;
  //
  // if (stat("register_list", &st) == -1)
  // {
  //     perror("stat in parse");
  //     return_value = -1;
  //     goto END_parse;
  // }
  // taille_fichier = st.st_size;
  //
  // f = fopen("register_list", "r");
  // if(f == NULL)
  // {
  //   fprintf(stderr, "Could not open %s\n", "register_list");
  //   perror("fopen in parse");
  //   return_value = -1;
  //   goto END_parse;
  // }
  //
  // registers = calloc(taille_fichier, sizeof(char**));
  // sizes_registers = calloc(taille_fichier, sizeof(i32));
  //
  // len_register = tokenize(f, registers, sizes_registers);
  // if (len_register == -1 || len_register == -3)
  // {
  //   return_value = -2;
  //   goto END_parse;
  // }
  // else if (len_register == -2)
  // {
  //   return_value = -3;
  //   goto END_parse;
  // }

  if (strcmp(in[0][0], "data:") == 0)
  {
    if (sizes[0] != 1)
    {
      if (printf("data: section is not correctly declared ; please make sure it only contains 'data:'\n") < 0)
      {
        perror("printf in parse");
      }
      return_value = false;
      goto END_parse;
    }

    *data_start = 0;
    ++indice_line;

    while(strcmp(in[indice_line][0], "code:") != 0 && indice_line < len)
    {
      // Lines in data must be 3 tokens
      if (sizes[indice_line] == 3)
      {
        if (good_variable(in[indice_line], opcodes, len_op) == false)
        {
          if (printf("Invalid variable in data section\n") < 0)
          {
            perror("printf in parse");
          }
          return_value = false;
          goto END_parse;
        }

      }
      ++indice_line;
    }
    if (indice_line == 0)
    {
      // Do something if data section is empty? TODO
    }
  }
  else
  {
    *data_start = -1;
  }

  if (strcmp(in[indice_line][0], "code:") == 0 && indice_line < len)
  {
    if (sizes[indice_line] != 1)
    {
      if (printf("code: section is not correctly declared ; please make sure it only contains 'code:'\n") < 0)
      {
        perror("printf in parse");
      }
      return_value = -1;
      goto END_parse;
    }
    else
    {
      *code_start = indice_line;
      ++indice_line;
    }

    i32  tmp;
    i32  i_data;
    bool parallel_on = false;
    while (indice_line < len)
    {
      tmp = good_instruction(in, sizes, indice_line,
                             opcodes, len_op,
                             *data_start, *code_start,
                             registers, sizes_registers, len_register,
                             labels, indice_labels,
                             requested_labels, indice_req, &parallel_on);
      if (tmp == -1)
      {
        if (printf("An error occured, exiting.\n") < 0)
        {
          perror("printf in parse");
        }
        for (i32 i = 0; i < *indice_labels; ++i)
        {
          free(labels[i]);
        }
        for (i32 i = 0; i < *indice_req; ++i)
        {
          free(requested_labels[i]);
        }
        free(labels);
        free(requested_labels);
        return_value = -1;
        goto END_parse;
      }
      else if (tmp == false)
      {
        return_value = false;
        goto END_parse;
      }
      else if (tmp == true)
      {
        return_value = true;
      }
      ++indice_line;
    }

  }

  else
  {
    if (printf("Could not detect code section.\n") < 0)
    {
      perror("printf in parse");
    }
    return_value = false;
    goto END_parse;
  }


  // Checking if every requested label has been defined
  i32 cpt = 0;
  for (i32 i = 0; i < *indice_req; ++i)
  {
    for (i32 j = 0; j < *indice_labels; ++j)
    {
      if (strcmp(requested_labels[i], labels[j]) == 0)
      {
        ++cpt;
        break;
      }
    }
  }
  if (cpt != *indice_req)
  {
    if (printf("At least one jump goes to an undefined label.\n") < 0)
    {
      perror("printf in parse");
    }
    return_value = false;
  }

  END_parse:
  // for (u64 i = 0; i < len_register; ++i)
  // {
  //   for (u64 j = 0; j < sizes_registers[i]; ++j)
  //   {
  //     free(registers[i][j]);
  //   }
  //   free(registers[i]);
  // }
  // free(registers);
  // free(sizes_registers);
  return return_value;
}
