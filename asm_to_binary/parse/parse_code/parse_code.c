#include "parse_code.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//Booleans define to make the code more readable
//Might be defined somewhere else so might have to remove it from here
#define bool int
#define TRUE 1
#define FALSE 0

bool is_a_letter_or_number(int str)
{
  // Check is ASCII code of character corresponds to a letter.
  return ((str >= 65 && str <= 90) || (str >= 97 && str <= 122) || (str >= 48 && str <=57));
}

// str_out is a buffer of size sizeof(char) * ((strlen(str_in) + 2))
// define it with char str_out[sizeof(char) * ((strlen(str_in) + 2))]
void add_comma(const char* str_in, char* str_out)
{
  int taille = (sizeof(char) * ((strlen(str_in))));
  int have_read_a_letter = FALSE;
  
  for (int i = 0; i < taille; ++i)
  {
    //~ printf("%d %d %d\n", str_in[i], i, have_read_a_letter);
    // Detect the beginning of first word
    if (have_read_a_letter == FALSE &&
        is_a_letter_or_number(str_in[i]) == TRUE)
    {
      have_read_a_letter = TRUE;
    }

    // Add a comma if first word ends
    if (have_read_a_letter == TRUE &&
        is_a_letter_or_number(str_in[i]) == FALSE)
      {
        char* str_tmp = (char*)malloc(taille + sizeof(char));
        strcpy(str_tmp, str_in);
        memmove(str_tmp+i, str_tmp+i-1, taille-i+1);
        str_tmp[i] = ',';
        strcpy(str_out, str_tmp);
        free(str_tmp);
        return;
      }
  };

  // Add a comma at the end if ends with first word
  if (have_read_a_letter)
  {
    char* str_tmp = (char*)malloc(taille + sizeof(char));
    strcpy(str_tmp, str_in);
    str_tmp[taille] = ',';
    strcpy(str_out, str_tmp);
    free(str_tmp);
    return;
  }

  // No word -> No modification
  return;
}
