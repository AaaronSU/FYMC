#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "tools.h"



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

    return TRUE;
}



/// @brief Function used to load in memory a file. This is practical performance wise.
/// @param filename Name of the file to load in memory
/// @return A string containing the whole file
char* load_file(char* filename)
{
    int file_size; //Size of the file, used for malloc
    FILE* fileptr = fopen(filename,"r"); //Pointer of the file

    //We go to the end of the file
    fseek(fileptr,0L,SEEK_END);

    //Now we retreive where we are (eq. the size of the file)
    file_size = ftell(fileptr);

    //Now we just copy the whole file into a buffer
    char* buffer = (char*)malloc(sizeof(char)*(file_size+1)); //Setting up the buffer, +1 for \0 just to be sure.
    rewind(fileptr); //Going back to the beginning of our stream
    fread(buffer,sizeof(char),file_size,fileptr);
    buffer[file_size+1] = '\0';

    return buffer;
}



/// @brief Function that load the op code data into few variables
/// @param op_name Will store the op code name (ex : add)
/// @param arg_count Will store how many args an op code take (ex : 2)
bool load_op_code(char* op_name, int* arg_count)
{
    //The file that contains the op code is always "op_codes"
    //Does the file exist ?
    if(access("op_codes",F_OK) != 0)
    {
        fprintf(stderr,"Error : the op_codes file is missing, cannot proceee further.\n");
        return FALSE;
    }

    //Now we read line by line, each line is an op code data
    FILE* fileptr = fopen("op_codes","r");
    char line[32]; //A line (whole op code data for a specific op code)
    int i = 0; //Current line number
    strcpy(op_name,"\0"); //Emptying the string
    while(fgets(line,32,fileptr) != NULL) //Until there si no more line to read
    {
        //Here line contains the last line readed.
        strcat(op_name,strtok(line,";")); //Adding op name
        strcat(op_name,";"); //Adding ; as separator
        arg_count[i] = atoi(strtok(NULL,"\n"));
        i++;
    }

    return TRUE;
}



/// @brief Function used to print a file loaded in memory (mostly for debug purposes)
/// @param Pointer of the file loaded in memory
void print_file(char* file)
{
    int i = 0;
    while(file[i] != '\0')
    {
        printf("%c", file[i]);
        i++;
    }
}