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

char* add_comma(char* str_in)
{
  int taille = (sizeof(char) * strlen(str_in));
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
        // MALLOC -> FREE THE FUCKING MEMORY !!!!!!
        char* str_out = (char*)malloc(taille + sizeof(char)); 
        strcpy(str_out, str_in);
        memmove(str_out+i, str_out+i-1, taille-i+1);
        str_out[i] = ',';
        return str_out;
      }
  };

  // MALLOC -> FREE THE FUCKING MEMORY !!!!!!
  // Add a comma at the end if no word
  if (have_read_a_letter)
  {
    char* str_out = (char*)malloc(taille + sizeof(char)); 
    strcpy(str_out, str_in);
    str_out[taille] = ',';
    return str_out;
  }
  
  // No word -> No modification
  // DON'T FREE IF NO MODIFICATION
  // Might need to malloc anyway for simpler code ?
  return str_in;
}
