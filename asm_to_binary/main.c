#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "parse.h" //Incluera data_parse.h et code_parse.h
#include "compile.h" //Incluera header_compile.h et code_compile.h

//Booleans define to make the code more readable
#define bool int
#define TRUE 1
#define FALSE 0

//Data readed from the op data file
char** op_name;
int* arg_count;
//bool* parenthesis;

/// @brief Function used to verify if argc >= 2 (i.e. their is a filename in argv[1]) and to test if the file does exist.
/// @param argc Number of args passed in command line
/// @param argv Values of args
/// @return True if everything is correct, false if argv < 2 OR argv[1] file does not exist
bool file_verification(int argc, char** argv)
{
    //Is their a file name as argument ?
    if(argc < 2)
    {
        fprintf(stderr,"USAGE : ./asm [FILENAME]\n");
        return FALSE;
    }

    //Does the file in argv[1] exist ?
    if(access(argv[1],F_OK) != 0)
    {
        fprintf(stderr,"Error : the file %s does not exist.\n", argv[1]);
        return FALSE;
    }
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

    //Initialization of operand code behaviour
    arg_count_init();

    if(parse() == FALSE)
    {
        return EXIT_FAILURE;
    }

    compile();

    return EXIT_SUCCESS;
}

/*
LISTE DES FONC :

1) load_file(str) : charge en mémoire l'intégralité du fichier
2) load_op_code() : charge en mémoire les données des op code 

3) char* add_comma(str) = ajoute une virgule après le premier "mots"
4) char* remove_space(str) = supprime l'intrégalité des espaces
5) char** retreive_token(str,char) = découpe la chaine en token avec , comme séparateur

    = donne quelque chose comme token["add","U1","U2",NULL]; (ne pas oublier le NULL qui indique la fin des tokens)

6) bool detect_alias(char**) = vérifie s'il y a un token qui termine par :
7) bool correct_alias(char**) = vérifie s'il l'alias est correct (aka présent à l'indice 0 et pas d'autres tokens)

8) bool detect_op_code(char*) = vérifie que token[0] correspond au nom d'un op code
9) bool correct_op_code(char**) = vérifie le bon nombre d'arguments ainsi que le bon nom des registres

    10) bool correct_register_name(char**) = verifie que les noms de registres existent

11) char** parse(str) : s'occupe de vérifie l'intrégité de la ligne (partie code pas data pour l'instant).
        
        parse appelera parse_data puis parse_code
        parse_code contiendra les fonctions de 3 à 10 et parse_data en réutilisera certaines

*/
