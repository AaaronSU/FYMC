#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//Booleans define to make the code more readable
#define bool int
#define TRUE 1
#define FALSE 0



/// @brief Function used to verify if argc >= 2 (i.e. their is a filename in argv[1]) and to test if the file does exist.
/// @param argc Number of args passed in command line
/// @param argv Values of args
/// @return True if everything is correct, false if argv < 2 OR argv[1] file does not exist
bool file_verification(int argc, char** argv)
{
    //Is their a file name as argument ?
    if(argc < 2)
    {
        fprintf(stderr,"USAGE : ./bin [FILENAME]\n");
        return FALSE;
    }

    //Does the file in argv[1] exist ?
    if(access(argv[1],F_OK) != 0)
    {
        fprintf(stderr,"Error : the file %s does not exist.\n", argv[1]);
        return FALSE;
    }
}



/// @brief Main function, used to launch big part of the binary to asm traduction
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
}

//Étape 1 : vérifier que le header est correcte avant de remettre en assembleur