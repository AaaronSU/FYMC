#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
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
        printf("'%s',",tokens[i++]);
    }
    printf("NULL]\n");
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



// //Retreiving token list from a string
char** retreive_token(char* line, char const separator)
{
    //MAX 128 elements per line
    //NOTE Changed malloc to calloc, seems like my intuition was right on that

    char** tokens = calloc(128, sizeof(char*));
    for(int i = 0; i < 128; i++)
    {
        //128 char max per token
        tokens[i] = calloc(128, sizeof(char));
    }
    //   V       | strlen = 6 CONTRE 9
    //ADD;U2;U3  |
    char* line_temp = line;
    int i = 0;
    int is_ascii = 0;
    while((line_temp = strchr(line,separator)) != NULL)
    {
        if (line[0] == '"')
        {
            // Get size of ascii
            size_t quote = 1;
            while (line[quote] != '"')
                ++quote;
            strncpy(tokens[i], line, quote + 1);
            // Considering quotes are only in ascii, we break here
            break;
        }

        //Copy of the string before the caracter we found
        strncpy(tokens[i], line, strlen(line) - strlen(line_temp));
        line = line_temp + 1; //Skipping the seperator
        i = i + 1;
    }

    //NULL means no more separator, i.e. last token
    if (line[strlen(line) - 1] == '\n')
        strncpy(tokens[i], line, strlen(line) - 1);
    else
        strncpy(tokens[i], line, strlen(line));
    // tokens[i] = line;
    // char* slash_n = strchr(line,'\n');
    // slash_n[0] = '\0';
    // WARNING We're making tokens[i+1] a NULL ptr so we free old memory
    free(tokens[i+1]);
    tokens[i+1] = NULL;

    //Returning token list ended by NULL value
    return tokens;
}



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


/// @brief Function that add a ; at the end of each word and will replace , with ; (except some exception)
/// @param str line which we want to add ;
bool add_semicolon(char* str)
{
    int i = 0;
    while(i < strlen(str))
    {
        //Case 1 : its a space, we ignore it
        // Added tab to it (ASCII 9 in table)
        // NOTE: Maybe put that as an else?
        if(str[i] == ' ' || str[i] == 9)
        {
            i++;
            continue;
        }

        //Case 2 : its a word, we analyze it
        // Added isdigit here for immediate values
        // Added a couple cases
        if(isalpha(str[i]) || isdigit(str[i]) || str[i] == '$' || str[i] == '@'
            || str[i] == '_' || str[i] == '-')
        {
            i++; //Next char
            //We add the ; at the end of the word
            while(str[i] != ' ')
            {
                //If there is a , between two word we replace it by a ;
                if(str[i] == ',') //This will also mark the end of the word
                {
                    str[i] = ' ';
                    break;
                }

                if(str[i] == '#') //End of the line
                {
                    str[i] = '\0';
                    return TRUE;
                }
                i++;
            }

            //We replace the " " with ; and go to the next char
            str[i++] = ';';
            continue;
        }

        //Case 3 : its a quote, i.e. beginning of a string
        if(str[i] == '\"')
        {
            i++; //We now search for the closing quote
            while(str[i] != '\"') //Closing quote
            {
                if(str[i] == '\0') //Missing closing quote
                {
                    fprintf(stderr,"Error : the string has no closing \".\n");
                    return FALSE;
                }

                i++;
            }

            //We are at the closing ", we go to the next char
            i++;
            //Is it the end of the words or are their still words after it ?
            if(str[i] == ' ') //I.e. not \0 so their are still some word after
            {
                str[i++] = ';';
            }

            ++i;
            continue; //Next char
        }

        //Case 4 : we read a #
        if(str[i] == '#') //End of the line
        {
            str[i] = '\0';
            return TRUE;
        }

        //Case 5 : its paranthesis
        if(str[i] == '(')
        {
            i++; //We now search for the parenthesis
            while(str[i] != ')') //Closing parenthesis
            {
                if(str[i] == '\0') //Missing closing parenthesis
                {
                    fprintf(stderr,"Error : the parenthesis are not closing.\n");
                    return FALSE;
                }

                i++;
            }

            //We are at the closing ", we go to the next char
            i++;
            //Is it the end of the words or are their still words after it ?
            if(str[i] == ' ') //I.e. not \0 so their are still some word after
            {
                str[i++] = ';';
            }

            ++i;
            continue; //Next char
        }

        //Case 6 : {}
        if(str[i] == '{')
        {
            i++;
            while(str[i] != '}') //Closing {}
            {
                if(str[i] == '\0') //Missing closing {}
                {
                    fprintf(stderr,"Error : the brace ({}) are not closing.\n");
                    return FALSE;
                }

                i++;
            }

            //We are at the closing ", we go to the next char
            i++;
            //Is it the end of the words or are their still words after it ?
            if(str[i] == ' ') //I.e. not \0 so their are still some word after
            {
                str[i++] = ';';
            }

            ++i;
            continue; //Next char
        }
    }

    return TRUE;
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
