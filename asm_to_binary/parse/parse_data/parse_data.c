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


//
bool has_only_valid_characters(char** array)
{
  int i = 0;
  size_t j = 0;
  while (array[i] != NULL)
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
