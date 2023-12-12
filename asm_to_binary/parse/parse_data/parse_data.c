#include "parse_data.h"
#include "../parse_code/parse_code.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Here we might want to load types from a file

/* 1) Vérifier synthaxe (nb args, séparateurs etc)
 *
 * 2) Vérifier que le type existe (dans 1 ?)
 *
 * 3à Vérifier que le nombre est bon
 *
 */


// Check if first two elements only contain letters / numbers / @
bool has_only_valid_characters(char** array)
{
  int i = 0;
  size_t j = 0;
  while (i < 2)
    {
      for (j = 0; j < strlen(array[i]); ++j)
      {
        if (!((array[i][j] >= 65 && array[i][j] <= 90)
            || (array[i][j] >= 97 && array[i][j] <= 122)
            || (array[i][j] >= 48 && array[i][j] <=57))
            && array[i][j] != 64)
          return FALSE;
      }
      ++i;
    }
  return TRUE;
}


//
bool seems_valid(char** array)
{
  int i = 0;

  // You can't have an alias in data section I believe
  if (!has_only_valid_characters(array))
    return FALSE;

  while (array[i] != NULL)
      ++i;

  return i == 3;
}


// Check if a alt of a is in b
bool is_in_list(char* str, char** array)
{
  int i = 0;

  while (array[i] != NULL)
  {
    if (strcmp(str, array[i]) == 0)
      return TRUE;
    ++i;
  }
  return FALSE;
}


//
bool type_exists(char* str)
{
  // This array shall not be created in the function !
  char* types_array[5] = {"u64", "i64", "f64", "ascii", NULL};

  // Checks if element is a valid type
  return is_in_list(str, types_array);
}


//
bool name_is_valid(char* str)
{
  // This array shall not be created in the function !
  char* types_array[5] = {"u64", "i64", "f64", "ascii", NULL};

  // Check if element is not a type
  if (is_in_list(str, types_array))
    return FALSE;

  // There should only be letters, numbers or @ at this point
  // thus we just check if there's no weirdly placed @
  for (size_t i = 1; i < sizeof(str); ++i)
  {
    if (str[i] == 64)
      return FALSE;
  }

  return TRUE;
}
