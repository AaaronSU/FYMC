#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "tools/tools.h"
#include "parse/parse.h"
#include "assembly/assembly.h"







#include <string.h>
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
	for(int i = 0; i <= number_of_args; i++)
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

	if(tokens[number_of_args+1] != NULL)
	{
		return FALSE; //i.e. to many args
	}

	return TRUE;
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









/// @brief Main function, used to launch big part of the asm to binary traduction
/// @param argc Number of arguments in the command line
/// @param argv Values of arguments in the command line
/// @return 0 if everything went well, 1 if something went wrong
int main(int argc, char** argv)
{
    //Is argv[1] is a valid file ?
    if(file_verification(argc, argv) == FALSE)
    {
        return EXIT_FAILURE;
    }

    //We load the op codes
    //To do so, we first need to create what will store the op code data
    char* op_name_list = (char*)malloc(sizeof(char)*128*8); //Name of the op code
    int* op_arg_count = (int*)malloc(sizeof(int)*128); //Number of arguments the op code can take
    if(load_op_code(op_name_list,op_arg_count)==FALSE)
    {
        return EXIT_FAILURE;
    }



    int place;
    char str[] = "add;U1;U2";
    char** tokens = retreive_token(str,";");

    if((place = detect_op_code(tokens[0],op_name_list)) == -1)
    {
        printf("ERROR %s DOES NOT EXIST\n",tokens[0]);
    }

    if(!correct_op_code(tokens,op_arg_count[place],"TEMP"))
    {
        printf("Error : args not correct\n");
    }











    //We load the file to assemble in memory
    char* file = load_file(argv[1]);

    //print_file(file);

    //Initialization of operand code behaviour
    // if(parse() == FALSE)
    // {
    //     return EXIT_FAILURE;
    // }

    // assembly();

    //We dont need the file loaded in memory anymore
    free(file);

    return EXIT_SUCCESS;
}

/*
LISTE DES FONC :

1) load_file(str) : charge en mémoire l'intégralité du fichier OK
2) load_op_code() : charge en mémoire les données des op code OK

3) char* add_comma(str) = ajoute une virgule après le premier "mots" OK
4) char* remove_space(str) = supprime l'intégralité des espaces OK
5) char** retreive_token(str,char) = découpe la chaine en token avec , comme séparateur OK

    = donne quelque chose comme token["add","U1","U2",NULL]; (ne pas oublier le NULL qui indique la fin des tokens)

6) bool detect_alias(char**) = vérifie s'il y a un token qui termine par : OK
7) bool correct_alias(char**) = vérifie s'il l'alias est correct (aka présent à l'indice 0 et pas d'autres tokens) OK

8) bool detect_op_code(char*) = vérifie que token[0] correspond au nom d'un op code
9) bool correct_op_code(char**) = vérifie le bon nombre d'arguments ainsi que le bon nom des registres

    10) bool correct_register_name(char**) = verifie que les noms de registres existent

11) char** parse(str) : s'occupe de vérifie l'intrégité de la ligne (partie code pas data pour l'instant).

        parse appelera parse_data puis parse_code
        parse_code contiendra les fonctions de 3 à 10 et parse_data en réutilisera certaines

*/
