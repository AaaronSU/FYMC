#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../parse/parse.h"
#include "tools.h"


//
void remove_space(char const *str_in, char *str_out)
{
    // Allocating memory and initialiazin looping elements
    int taille_in = (sizeof(char) * ((strlen(str_in)))) + sizeof(char);
    int taille_out = (sizeof(char) * ((strlen(str_out)))) + sizeof(char);
    char* str_tmp = calloc(taille_out, sizeof(char));

    int i = 0;
    int j = 0;

    int quote_flag = FALSE;

    while (i < taille_in)
    {
        if(str_in[i] == '\"')
        {
            if(quote_flag)
            {
                quote_flag = FALSE;
            }
            else
            {
                quote_flag = TRUE;
            }
        }

        // 32 is space's ASCII code, 9 is end of line
        if((str_in[i] != ' ' && str_in[i] != 9 && str_in[i] != 10) || quote_flag)
        {
            str_out[j] = str_in[i];
            ++j;
        }
        ++i;
    }
    free(str_tmp);
}



/// @brief Function used to verify if argc >= 2 (i.e. their is a filename in argv[1]) and to test if the file does exist.
/// @param argc Number of args passed in command line
/// @param argv Values of args
/// @return True if everything is correct, false if argv < 2 OR argv[1] file does not exist
bool file_verification(int const argc, char** const argv)
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
char* load_file(char* const filename)
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



/// @brief Function used to print a file loaded in memory (mostly for debug purposes)
/// @param Pointer of the file loaded in memory
void print_file(char* const file)
{
    int i = 0;
    while(file[i] != '\0')
    {
        printf("%c", file[i]);
        i++;
    }
}



/// @brief print tokens list (print each char* until NULL is read, will also print the separators)
/// @param tokens List of tokens
/// @param separator separator used to retreive tokens, will also be printed back
void print_tokens_line(char** const tokens)
{
    int i = 0;
    printf("LINE : [");
    while(tokens[i] != NULL)
    {
        printf("%s,",tokens[i++]);
    }
    printf("NULL]\n");
}


typedef struct {
    void* array;
    long long int ipu;
} conteneur;



/// @brief function that load all the registers saved in a file. They will be used for parsing
/// @return the list of token of for each register
char*** tokenize(char* const fileName)
{
    if(access(fileName,F_OK) != 0)
    {
        fprintf(stderr,"Error: the %s file is missing, cannot proceed further.\n",fileName);
        abort();
    }

    char*** token_list = calloc(128, sizeof(char**));

    FILE* fileptr = fopen(fileName,"r");
    char line[128];
    int i = 0;

    // TODO handle more than 128 lines
    while(fgets(line,127,fileptr) != NULL)
    {
        // printf("LINE TO TOKENIZE : %s",line);
        token_list[i] = retreive_token(line,';');
        // print_tokens_line(token_list[i]);
        // printf("\n");
        i++;
    }
    token_list[i] = NULL;

    fclose(fileptr);
    return token_list;
}



void print_tokens_list(char*** const token_list)
{
    //When we find an empty line we stop
    int i = 0;
    while(token_list[i] != NULL)
    {
        print_tokens_line(token_list[i]);
        i++;
    }
}


void free_char2(char** char2, int size_dim_1)
{
    //If already empty
    if(char2 == NULL)
    {
        return;
    }

    for(int i = 0; i < size_dim_1; i++)
    {
        if(char2[i] != NULL)
        {
            free(char2[i]);
        }
    }
    free(char2);
}


void free_char3(char*** char3, int size_dim_1, int size_dim_2)
{
    //If already empty
    if(char3 == NULL)
    {
        return;
    }

    for(int i = 0; i < size_dim_1; ++i)
    {
        if(char3[i] != NULL)
        {
            for(int j = 0; j < size_dim_2; ++j)
            {
                if(char3[i][j] != NULL)
                {
                    free(char3[i][j]);
                }
            }
            free(char3[i]);
        }
    }
    free(char3);
}
