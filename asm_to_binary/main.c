#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "tools/tools.h"
#include "parse/parse.h"
#include "assembly/assembly.h"

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
    char* op_name = (char*)malloc(sizeof(char)*128*8); //Name of the op code
    int* arg_count = (int*)malloc(sizeof(int)*128); //Number of arguments the op code can take
    if(load_op_code(op_name,arg_count)==FALSE)
    {
        return EXIT_FAILURE;
    }

    //We load the file to assemble in memory
    char* file = load_file(argv[1]);

    print_file(file);

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

3) char* add_comma(str) = ajoute une virgule après le premier "mots"
4) char* remove_space(str) = supprime l'intégralité des espaces
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