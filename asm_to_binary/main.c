#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "opcode.h"
#include "parse.h" //Incluera data_parse.h et code_parse.h
#include "compile.h" //Incluera header_compile.h et code_compile.h

//Booleans define to make the code more readable
#define bool int
#define TRUE 1
#define FALSE 0

//Operand's code
#define NBR_OP 2 //Total number of operand
#define ADD 0
#define SUB 1

typedef struct
{
    int arg_count;
    bool parenthesis;
} op_data_t;

//Operand data will be stored here
op_data_t op_data[NBR_OP];



/// @brief Function used to init op_data array so we will now how to act when reading a specific operand
void arg_count_init()
{
    //ADD
    op_data[ADD].arg_count = 2;
    op_data[ADD].parenthesis = FALSE;

    //SUB
    op_data[SUB].arg_count = 2;
    op_data[SUB].parenthesis = FALSE; 

    //...
}



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
 *  À demander à YASPR :
 *
 *      --> Demander une explication détaillé sur load (notamment l'offset)
 *      --> Dans storeu c'est une erreur le fait de mettre u0 alors que c'est censé etre u2 ?
 *      --> Pourquoi dans l'example 3 on a storeu avec des parenthèse à droite ?
 *      --> parenthèses et crochets (pour load et indirection) 
 *      --> Montrer la structure de donnée choisie
 *      --> Demander s'il n'y a bien que 3 programmes à faire : assembleur, déassembleur et vm
 *      --> Demander si le langage C est bien le plus approprié pour realiser ces 3 fichiers
 *      --> Demander comment gérer la partie data (et quels sont les données que l'on passe dedans)
 */
