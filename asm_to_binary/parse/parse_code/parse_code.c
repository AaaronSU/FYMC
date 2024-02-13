#include "parse_code.h"
#include "../../tools/tools.h"
#include "../parse_data/parse_data.h"
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
      // We use strchr here 'cause there's no : or ascii at this point
      if (strchr(array[i], ':'))
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
	printf("%s\n", tmp);
	for(int i = 0; tmp[i]; i++){
		tmp[i] = toupper(tmp[i]);
	}

  	for (int i = 0; op_name_list[i] != NULL; i++)
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




bool correct_parenthesis(char* reg, char** types, char*** register_list)
{
	bool is_ok = FALSE;

	// Check if parenthesis are allowed for this register
	for (size_t i = 0; types[i] != NULL; ++i)
	{
		if (strcmp(types[i], "par") == 0)
			{
				is_ok = TRUE;
				break;
			}
	}

	if (is_ok == FALSE)
		return FALSE;

	// Tokenizing
	char* char_tmp = calloc(strlen(reg), sizeof(char));
	strncpy(char_tmp, reg+1, strlen(reg)-2);
	strcat(char_tmp, "\0");

	char** token_thing = retreive_token(char_tmp, ',');
	free(char_tmp);

	if (token_thing[3] != NULL)
		{
			free(token_thing);
			return FALSE;
		}


	// Retreive max number of register
	int register_range = -1;
	char** types_list = NULL;
	for(int i = 0; register_list[i] != NULL; i++)
	{
		if(strcmp("U",register_list[i][0]) == 0)
		{
			//We are at the right place in the list
			//We can retreive datas
			register_range = atoi(register_list[i][1]);
			types_list = &register_list[i][2];
			break;
		}
	}
	// If CPU doesn't handle unsigned for some reason
	if (register_range == -1)
		{free(token_thing); return FALSE;}


	// Check if registers are correct (must be unsigned)
	for (size_t i = 0; i < 2; ++i)
	{
		if (token_thing[i] != NULL)
		{
			if (toupper(token_thing[i][0]) == 'U')
			{
				char* tmp = calloc(strlen(token_thing[i]), sizeof(char));
				strncpy(tmp, token_thing[i]+1, strlen(token_thing[i]));

				if(atoi(tmp) < 0 || atoi(tmp) > register_range)
				{
					if(atoi(tmp) < 0)
					{
						fprintf(stderr,"Error : register number too low (must be between 0 and %d).\n",register_range);
					}
					else
					{
						fprintf(stderr,"Error : register number too high (must be between 0 and %d).\n",register_range);
					}
					free(tmp);
					free(token_thing);
					return FALSE;
				}
				free(tmp);
			}
			else
				{free(token_thing); return FALSE;}
		}
		else
			{free(token_thing); return FALSE;}
	}

	bool answer = TRUE;
	if (token_thing[2] != NULL)
		{
			answer = good_integer(token_thing[2], TRUE);
			// We write immediate values on 64 bits
			// TODO: check if those values are reachable i.e. if I need to put <=
			if (atoi(token_thing[2]) > 2147483647 || atoi(token_thing[2]) < -2147483647)
				answer = FALSE;
		}

	free(token_thing);
	return answer;
}




bool correct_brace(char* reg, char** types, char*** register_list)
{
	bool is_ok = FALSE;

	// Check if parenthesis are allowed for this register
	for (size_t i = 0; types[i] != NULL; ++i)
	{
		if (strcmp(types[i], "brace") == 0)
			{
				is_ok = TRUE;
				break;
			}
	}

	if (is_ok == FALSE)
		return FALSE;

	// Tokenizing
	char* char_tmp = calloc(strlen(reg), sizeof(char));
	strncpy(char_tmp, reg+1, strlen(reg)-2);
	strcat(char_tmp, "\0");

	char** token_thing = retreive_token(char_tmp, ',');
	free(char_tmp);

	if (token_thing[8] != NULL)
	{
		free(token_thing);
		return FALSE;
	}

	char* type = types[2];
	int type_int = -1;
	if (strcmp(type, "u64") == 0)
		type_int = 0;
	if (strcmp(type, "s64") == 0)
		type_int = 1;
	if (strcmp(type, "f64") == 0)
		type_int = 2;

	if (type_int == -1)
		{free(token_thing); return FALSE;}

	int i = 0;
	bool answer = FALSE;

	while (token_thing[i] != NULL)
	{
		if (type_int == 0)
			answer = good_integer(token_thing[i], FALSE);
		if (type_int == 1)
			answer = good_integer(token_thing[i], TRUE);
		if (type_int == 2)
			answer = good_float(token_thing[i]);
		++i;
	}

	free(token_thing);

	if (i != 8)
		answer = FALSE;
	return answer;
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

	// Handling immediate values
	if (isdigit(reg[i]))
		return FALSE;

	// Handling parenthesis
	if (reg[0] == '(')
		return correct_parenthesis(reg, types, register_list);


	// Handling braces
	if (reg[0] == '{')
		return correct_brace(reg, types, register_list);

	while(isalpha(reg[i]))
	{
		// We're stocking register types in upper case
		register_type[i] = toupper(reg[i]);
		i++;
	}
	register_type[i] = '\0';

	i = 0;

	if(register_list[i][0] == NULL)
	{
		fprintf(stderr,"Error : no register begin with %s.\n",register_type);
		return FALSE;
	}

	while(register_list[i] != NULL)
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



bool correct_immediate(char** tokens, int i, char** op_code_datas, char*** data_array)
{
	// Immediate value must be the last one
	if (tokens[i+1] == NULL)
	{
		for (size_t k = 2; op_code_datas[k] != NULL; ++k)
		{
			char* str_tmp = calloc(strlen(op_code_datas[k]) + 1, sizeof(char));
			remove_space(op_code_datas[k], str_tmp);

			// We can't have immediate ascii now can we ? (change this if we can)
			if (strcmp(str_tmp, "ascii") != 0)
			{
				if (strcmp(str_tmp, "u64") == 0)
				{
					free(str_tmp);
					return good_integer(tokens[i], FALSE);
				}
				else if (strcmp(str_tmp, "s64") == 0)
				{
					free(str_tmp);
					return good_integer(tokens[i], TRUE);
				}
				else if (strcmp(str_tmp, "f64") == 0)
				{
					free(str_tmp);
					return good_float(tokens[i]);
				}
			}
			free(str_tmp);
		}

	}
	return FALSE;
}



bool correct_variable(char** tokens, int i, char** op_code_datas, char*** data_array)
{
	// Can't be correct if no data to begin with
	if (data_array != NULL)
	{
		int taille = strlen(tokens[i]);
		char* str_tmp = calloc(taille + 1, sizeof(char));

		if (tokens[i][0] == '$')
				strncpy(str_tmp, tokens[i]+1, taille);
		else
			strncpy(str_tmp, tokens[i], taille+1);

		// Checking if requested token is in data_array
		for (size_t k = 0; data_array[k] != NULL; ++k)
		{
			if (strcmp(str_tmp, data_array[k][1]) == 0)
			{
				// Checking variable's type
				if (tokens[i][0] == '@')
				{
					for (size_t l = 2; op_code_datas[l] != NULL; ++l)
					{
						if (strcmp(op_code_datas[l], "u64") == 0 || strcmp(op_code_datas[l], "ascii") == 0)
						{
							free(str_tmp);
							return TRUE;
						}
					}
				}

				else
				// Other cases (only $ for now but maybe it'll change, who knows...)
				for (size_t l = 2; op_code_datas[l] != NULL; ++l)
				{
					if (strcmp(op_code_datas[l], data_array[k][0]) == 0)
					{
						free(str_tmp);
						return TRUE;
					}
				}
			}
		}

		free(str_tmp);
	}
	return FALSE;
}


bool is_in_address_array(char* name, char** address_array)
{
	int int_tmp = atoi(address_array[0]);
	if (int_tmp == 0)
		return FALSE;
	for (size_t i = 1; i <= int_tmp; ++i)
	{
		if (strcmp(address_array[i], name) == 0)
			return TRUE;
	}
	return FALSE;
}



bool correct_op_code(char** tokens, char** op_code_datas, char*** register_list, char*** data_array, char** address_array)
{
	int argc = atoi(op_code_datas[1]);
	for(int i = 1; i <= argc; i++)
	{
		if(tokens[i] == NULL)
		{
			return FALSE; //i.e. not enough args
		}

		// Handling jump commands
		if (strcmp(op_code_datas[2], "address") == 0)
		{
			if (is_in_address_array(tokens[i], address_array) == FALSE)
				return FALSE;
		}

		else if (tokens[i][0] == '$' || tokens[i][0] == '@')
		{
			//Check if variable exists
			return correct_variable(tokens, i, op_code_datas, data_array);
		}

		//We need to verify if the name of the register is correct
		//tokens[i] because with skip op name and op arg count, we just send the types of the op code
		else if(!correct_register_name(tokens[i],&op_code_datas[2],register_list))
		{
			return correct_immediate(tokens, i, op_code_datas, data_array);
		}
	}

	if(tokens[argc+1] != NULL)
	{
		return FALSE; //i.e. to many args
	}

	return TRUE;
}


bool correct_line(char** line, char*** op_name_list, char*** register_list, char*** data_array, char** address_array)
{
	// Vérifie les alias
	if (detect_alias(line) == TRUE)
	{
		if (correct_alias(line) == FALSE)
			return FALSE;

		// Alias can't be followed by anything besides blank line
		if (line[1])
			return FALSE;

		// Removing final ':'
		int taille = strlen(line[0]);
		int int_tmp = atoi(address_array[0]);
		char* tmp = calloc(taille, sizeof(char));
		strncpy(tmp, line[0], taille - 1);
		strcat(tmp, "\0");

		// print_tokens_line(line);
		if (is_in_address_array(tmp, address_array))
			return FALSE;

		// Adding address to array
		strcpy(address_array[int_tmp], tmp);

		// Converting integer to string
		char* buf = calloc(2, sizeof(char));
		sprintf(buf, "%d", ++int_tmp);

		// Updating array size
		strncpy(address_array[0], buf, 2);

		free(buf);
		free(tmp);
		return TRUE;
	}

	// Vérifie op_code et registres
	int position = 0;
	position = detect_op_code(line[0], op_name_list);
	if (position == -1)
		return FALSE;
	return correct_op_code(line, op_name_list[position], register_list, data_array, address_array);
}
