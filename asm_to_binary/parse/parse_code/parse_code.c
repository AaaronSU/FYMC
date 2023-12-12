#include "parse_code.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

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
/// @return return the line where the data of the op code are stored (-1 if the op code does not exist)
int detect_op_code(char* op_name, char*** op_name_list)
{
  	for (int i = 0; op_name_list[i][0] != NULL; i++)
  	{
    	if(strcmp(op_name,op_name_list[i][0]) == 0)
		{
			return i;
		}
  	}

	//No op code with the same name found, returning false
	return -1; //-1 means not a valid op code
}



// /// @brief function that tell if the register are correct
// /// @param tokens list of all the register and the op code
// /// @param register_list list of all the possible register and their data type associated
// /// @return return true if everything is good, else return false
bool correct_register_name(char* reg, char** types, char*** register_list)
{	
	//We have the name of the register, the type(s) the register can take,
	//now we need to check if they match

	//Step 1 : a register begin with U,S,F,V,T or G
	char register_type = reg[0];

	if(register_type != 'U' &&
	   register_type != 'S' &&
	   register_type != 'F' &&
	   register_type != 'V' &&
	   register_type != 'T' &&
	   register_type != 'G'
	) {
		fprintf(stderr,"Error : no register begin with %c.\n",register_type);
		return FALSE;
	}

	//We must retreive the datas associated with this register
	int register_range;
	char** types_list = NULL;
	for(int i = 0; register_list[i] != NULL; i++)
	{
		if(register_list[i][0][0] == register_type)
		{
			//We are at the right place in the list
			//We can retreive datas
			register_range = atoi(register_list[i][1]);
			types_list = &register_list[i][2];
			break;
		}
	}

	//Step 2 : register number range
	//We first check if we only have numbers and not some letters (else the conversion from string to int may be wrong)
	if(reg[1] == '\0')
	{
		fprintf(stderr,"Error : invalid register number value (no number).\n");
		return FALSE;
	}

	int i = 1; //This take in account that register may have different number size
	while(reg[i] != '\0')
	{
		if(!isdigit(reg[i]))
		{
			fprintf(stderr,"Error : invalid register number value.\n");
			return FALSE;
		}
		i++;
	}

	//Step 3 : is the number between 1 and MAX VALUE ?
	if(atoi(&reg[1]) < 1 || atoi(&reg[1]) > register_range)
	{
		if(atoi(&reg[1]) < 1)
		{
			fprintf(stderr,"Error : register number too low (must be between 1 and %d).\n",register_range);
		}
		else
		{
			fprintf(stderr,"Error : register number too high (must be between 1 and %d).\n",register_range);
		}
		return FALSE;
	}

	//Step 4 : is the register type in accordance with the op code ?
	//We need to verify if the register type (ex:U12) match the type with the op code (ex:add with unsigned integer)
	for(int i = 0; types[i] != NULL; i++)
	{
		for(int j = 0; types_list[j] != NULL; j++)
		{
			printf("%s ?= %s\n",types[i],types_list[j]);
			if(strcmp(types[i],types_list[j]) == 0)
			{
				//The types match, every test are correct
				return TRUE;
			}
		}
	}

	//The types do not match
	fprintf(stderr,"Error : register type not matching with op code.\n");
	return FALSE;
}




bool correct_op_code(char** tokens, char** op_code_datas, char*** register_list)
{
	int argc = atoi(op_code_datas[1]);
	for(int i = 1; i <= argc; i++)
	{
		if(tokens[i] == NULL)
		{
			return FALSE; //i.e. not enough args
		}

		//We need to verify if the name of the register is correct
		if(!correct_register_name(tokens[i],&op_code_datas[2],register_list)) //tokens[2] because with skip op name and op arg count, we just send the types of the op code
		{
			return FALSE; //i.e. the register name is not correct
		}
	}

	if(tokens[argc+1] != NULL)
	{
		return FALSE; //i.e. to many args
	}

	return TRUE;
}