#include "parse_code.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//Booleans define to make the code more readable
//Might be defined somewhere else so might have to remove it from here
#define bool int
#define TRUE 1
#define FALSE 0

//
bool alphanumeric(int str)
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
        alphanumeric(str_in[i]) == TRUE)
    {
      have_read_a_letter = TRUE;
    }

    // Add a comma if first word ends
    if (have_read_a_letter == TRUE &&
        alphanumeric(str_in[i]) == FALSE)
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


//
int size_without_chara(const char* str,
                        int const taille,
                        int const chara)
{
  // Utiliser des pointeurs pour éviter les copies ? À voir
  int taille_out = taille;
  for (int i = 0; i < taille; ++i)
  {
    if (str[i] == chara)
      taille_out -= sizeof(char);
  }
  return taille_out;
}



//
void remove_space(const char* str_in, char* str_out)
{
  // Allocating memory and initialiazin looping elements
  int taille_in = (sizeof(char) * ((strlen(str_in)))) + sizeof(char);
  int taille_out = (sizeof(char) * ((strlen(str_out)))) + sizeof(char);
  //~ printf("%d\n", taille_out);
  char* str_tmp = (char*)malloc(taille_out);
  
  int i = 0;
  int j = 0;
  
  while (i < taille_in)
  {
    // 32 is space's ASCII code
    if (str_in[i] != 32)
    {
      str_tmp[j] = str_in[i];
      ++j;
    }
    ++i;
  }
  
  strcpy(str_out, str_tmp);
  free(str_tmp);
  
  return;
}

//Retreiving token list from a string
char** retreive_token(char* line, char* const separator)
{
    char** tokens = (char**)malloc(sizeof(char*)*128); //MAX 128 elements per line
	int i = 0; //Used to count tokens
	tokens[i] = strtok(line,separator); //Retreiving first token
	while(tokens[i] != NULL) //As long as we still have tokens
	{
		i = i + 1; //Next token count
		tokens[i] = strtok(NULL,separator); //Reading next token
	}

    //Returning token list ended by NULL value
    return tokens;
}



//
bool detect_alias(char** array)
{
  int i = 0;
  while (array[i] != NULL)
    {
      //~ printf("in the loop, i = %d\n", i);
      // 58 is ASCII code for ':'
      if (array[i][strlen(array[i]) - 1] == 58)
        return TRUE;
      ++i;
    }
    //~ printf("fin fonction\n");
  return FALSE;
}


//
// Check if alias is in first position and is the only alias in array
bool correct_alias(char** array)
{
  int i = 0;
  
  // If token is in first position
  if (array[i][strlen(array[i]) - 1] != 58)
    return FALSE;
  
  ++i;
  
  while (array[i] != NULL)
    {
      // 58 is ASCII code for ':'
      if (array[i][strlen(array[i]) - 1] == 58)
        return FALSE;

      ++i;
    }
  return TRUE;
}


/// @brief function that detect if a readen op code does exist
/// @param op_name name of the op code we want to verify
/// @param op_name_list list of all op codes name
/// @return return the place in the list of op code args where the number of args of our op code reside, else -1 if the op code does not exist
int detect_op_code(char* op_name, char* op_name_list)
{
	int i = 0; //Where is the number of args of the op code in the list of args
	char* token = strtok(op_name_list,";");
	while(token != NULL)
	{
		if(strcmp(token,op_name) == 0)
		{
			return i;
		}
		else
		{
			token = strtok(NULL,";");
			i = i + 1; //Next arg count in list
		}
	}

	//No op code with the same name found, returning false
	return -1; //-1 means not a valid op code
}



//verifie que les noms de registres existent
bool correct_register_name(char* tokens, char* register_list)
{
	return TRUE;
}



/// @brief Verify if the number of args our op code as is correct
/// @param tokens list of the tokens
/// @param number_of_args number of args the op code take
/// @return TRUE if the number is correct and the name of the registers are okay, else FALSE
bool correct_op_code(char** tokens, int number_of_args, char* register_list)
{
	for(int i = 1; i < number_of_args; i++)
	{
		if(tokens[i] == NULL)
		{
			return FALSE; //i.e. not enough args
		}

		//We need to verify if the name of the register is correct
		if(!correct_register_name(tokens[i],register_list))
		{
			return FALSE; //i.e. the register name is not correct
		}

		//Next token
		i++;
	}

	if(tokens[number_of_args] != NULL)
	{
		return FALSE; //i.e. to many args
	}

	return TRUE;
}