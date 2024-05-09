#include "parse_data.h"
// #include "../parse_code/parse_code.h"
// #include "../../tools/tools.h"
#include "../../tools/types.h"
#include <string.h>
// #include <stdio.h>
// #include <stdlib.h>

/// Returns true if the variable is valid, false if it is not
bool good_variable(char** line, char*** opcodes, i32 len_op)
{
  bool return_value = false;

  if (strcmp(line[0], "ascii") == 0)
  {
    return_value = true;
  }
  else if (strcmp(line[0], "u64") == 0)
  {
    return_value = good_integer(line[2], false);
  }
  else if (strcmp(line[0], "f64") == 0)
  {
    return_value = good_float(line[2]);
  }
  else if (strcmp(line[0], "s64") == 0)
  {
    return_value = good_integer(line[2], true);
  }
  else
  {
    return_value = false;
  }

  return return_value;
}



/// Returns if str is NOT a number or a sign symbol
/// (i.e. if str is a number or a signing symbol, returns false)
bool wrong_signing_symbol(char* str)
{
  return ((str[0] < 48 || str[0] > 57)  // pas un nombre
          && (str[0] != 43 && str[0] != 45)); // pas + ou -;
}


/// Returns true if str is an integer, false otherwise
///   is_signed determines if the integer can be signed
///   if is_signed is false, all characters from str must be digits
bool good_integer(char* str, bool is_signed)
{
  u64 i = 0;
  if (is_signed)
  {
    if (wrong_signing_symbol(str))
    {
      return false;
    }
    i = 1;
  }

  for (; i < strlen(str); ++i)
  {
    if (str[i] < 48 || str[i] > 57)
    {
      return false;
    }
  }
  return true;
}


/// Returns true if str is a float, false otherwise
bool good_float(char* str)
{
  u64 i = 0;
  if (wrong_signing_symbol(str))
  {
    return false;
  }
  i = 1;
  bool has_a_point = false;

  for (; i < strlen(str); ++i)
  {
    if ((str[i] < 48 || str[i] > 57))
    {
      if (str[i] == 46)
      {
        if (!has_a_point)
        {
          has_a_point = true;
        }
        else
        {
          return false;
        }
      }
      else
      {
        return false;
      }
    }
  }
  return true;
}
