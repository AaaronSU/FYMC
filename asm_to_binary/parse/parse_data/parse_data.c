#include "parse_data.h"
#include "../parse_code/parse_code.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Here we might want to load types from a file


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



bool wrong_signing_symbol(char* str)
{
  return ((str[0] < 48 || str[0] > 57)  // pas un nombre
        && (str[0] != 43 && str[0] != 45)); // pas + ou -;
}


//
bool good_integer(char* str, bool is_signed)
{
  size_t i = 0;
  if (is_signed)
  {
    if (wrong_signing_symbol(str))
      return FALSE;
    i = 1;
  }

  for (; i < strlen(str); ++i)
  {
    if (str[i] < 48 || str[i] > 57)
      return FALSE;
  }
  return TRUE;
}


//
bool good_float(char* str)
{
  size_t i = 0;
    if (wrong_signing_symbol(str))
      return FALSE;
    i = 1;

  bool has_a_point = FALSE;

  for (; i < strlen(str); ++i)
  {
    if ((str[i] < 48 || str[i] > 57))
    {
      if (str[i] == 46)
      {
        if (!has_a_point)
          has_a_point = TRUE;
        else
          return FALSE;
      }
      else
        return FALSE;
    }
  }
  return TRUE;
}


//ASCII variables must start and end with " and not have any inbetween
bool good_ascii(char* str)
{
  if(str[0] != '\"')
  {
    fprintf(stderr,"Error : the string does not begin with the caracter \".\n");
    return FALSE;
  }

  char* next_quote = strchr(&str[1],'\"'); //Skipping first "

  if(next_quote == NULL)
  {
    fprintf(stderr,"Error : no \" detected to end the string.\n");
    return FALSE;
  }

  if(next_quote[1] != '\0')
  {
    fprintf(stderr,"Error : \" detected which is not the quote that close the string.\n");
    return FALSE;
  }

  return TRUE;
}
