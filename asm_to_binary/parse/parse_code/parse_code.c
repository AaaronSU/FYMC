#include "parse_code.h"
#include "../../tools/tools.h"
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
	//NOTE
	// We use a capsed version of the op_code
	// Maybe do it with register names too, or not do it at all.
	char* tmp = calloc(strlen(op_name) + 1, sizeof(char));
	strcpy(tmp, op_name);
	for(int i = 0; tmp[i]; i++){
		tmp[i] = toupper(tmp[i]);
	}

  	for (int i = 0; op_name_list[i][0] != NULL; i++)
  	{
    	if(strcmp(tmp, op_name_list[i][0]) == 0)
			{
				free(tmp);
				return i;
			}
  	}

	free(tmp);
	//No op code with the same name found, returning false
	return -1; //-1 means not a valid op code
}



/// @brief function that tell if the register are correct
/// @param tokens list of all the register and the op code
/// @param register_list list of all the possible register and their data type associated
/// @return return true if everything is good, else return false
bool correct_register_name(char* reg, char** types, char*** register_list)
{
	//We have the name of the register, the type(s) the register can take,
	//now we need to check if they match

	//Step 1 : a register begin with U,S,F,V,T or G
	char register_type[128];
	int i = 0;
	while(isalpha(reg[i]))
	{
		register_type[i] = reg[i];
		i++;
	}
	register_type[i] = '\0';

	i = 0;
	while(register_list[i][0] != NULL)
	{
		if(strcmp(register_type,register_list[i][0]) == 0)
		{
			i++;
			break;
		}
		else
		{
			i++;
		}
	}

	if(register_list[i][0] == NULL)
	{
		fprintf(stderr,"Error : no register begin with %s.\n",register_type);
		return FALSE;
	}

	//We must retreive the datas associated with this register
	int register_range;
	char** types_list = NULL;
	for(int i = 0; register_list[i] != NULL; i++)
	{
		if(strcmp(register_type,register_list[i][0]) == 0)
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
	if(reg[strlen(register_type)] == '\0')
	{
		fprintf(stderr,"Error : invalid register number value (no number).\n");
		return FALSE;
	}

	i = strlen(register_type); //This take in account that register may have different number size
	while(reg[i] != '\0')
	{
		if(!isdigit(reg[i]))
		{
			fprintf(stderr,"Error : invalid register number value.\n");
			return FALSE;
		}
		i++;
	}

	// NOTE: it said between 1 and MAX VALUE but register can be 0 or 32 iirc
	//Step 3 : is the number between 0 and MAX VALUE ?
	if(atoi(&reg[strlen(register_type)]) < 0 || atoi(&reg[strlen(register_type)]) > register_range)
	{
		if(atoi(&reg[strlen(register_type)]) < 0)
		{
			fprintf(stderr,"Error : register number too low (must be between 0 and %d).\n",register_range);
		}
		else
		{
			fprintf(stderr,"Error : register number too high (must be between 0 and %d).\n",register_range);
		}
		return FALSE;
	}

	//Step 4 : is the register type in accordance with the op code ?
	//We need to verify if the register type (ex:U12) match the type with the op code (ex:add with unsigned integer)
	for(int i = 0; types[i] != NULL; i++)
	{
		for(int j = 0; types_list[j] != NULL; j++)
		{
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
		// If tokens[i] is a nullptr
		if (!(tokens[i]))
			return FALSE;

		if(tokens[i] == NULL)
		{
			return FALSE; //i.e. not enough args
		}

		//We need to verify if the name of the register is correct
		//TODO handle immediate values
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


bool correct_line(char** line, char*** op_name_list, char*** register_list)
{

	// Vérifie les alias
	if (detect_alias(line) == TRUE && correct_alias(line) == FALSE)
		return FALSE;
	// Vérifie op_code et registres
	int position = 0;
	position = detect_op_code(line[0], op_name_list);
	if (position == -1)
		return FALSE;
	return correct_op_code(line, op_name_list[position], register_list);
}





