#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "parse_code/parse_code.h"
#include "parse_data/parse_data.h"
#include "parse.h"



//
bool alphanumeric(int str)
{
  // Check is ASCII code of character corresponds to a letter.
  return ((str >= 65 && str <= 90) || (str >= 97 && str <= 122) || (str >= 48 && str <=57));
}



//
int size_without_chara(const char* str,
                        int const taille,
                        int const chara)
{
  // Utiliser des pointeurs pour éviter les copies ? À voir
  int taille_out = taille;
  for (int i = 0; i < taille; ++i)
  {
    if (str[i] == chara)
      taille_out -= sizeof(char);
  }
  return taille_out;
}



//
void remove_space(char const *str_in, char *str_out)
{
    // Allocating memory and initialiazin looping elements
    int taille_in = (sizeof(char) * ((strlen(str_in)))) + sizeof(char);
    int taille_out = (sizeof(char) * ((strlen(str_out)))) + sizeof(char);
    //~ printf("%d\n", taille_out);
    char* str_tmp = (char*)malloc(taille_out);
    
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

        // 32 is space's ASCII code
        if(str_in[i] != ' ' || quote_flag)
        {
            str_out[j] = str_in[i];
            ++j;
        }
        ++i;
    }
}



/// @brief Function that add a ; at the end of each word and will replace , with ; (except some exception)
/// @param str line which we want to add ;
bool add_semicolon(char* str)
{
    int i = 0;
    while(i < strlen(str))
    {
        //Case 1 : its a space, we ignore it
        if(str[i] == ' ')
        {
            i++;
            continue;
        }

        //Case 2 : its a word, we analyze it
        if(isalpha(str[i]))
        {
            i++; //Next char
            //We add the ; at the end of the word
            while(str[i] != ' ')
            {
                //If there is a , between two word we replace it by a ;
                if(str[i] == ',')
                {
                    str[i] = ';';
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
                    fprintf(stderr,"Error : the string as no closing \".\n");
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

            continue; //Next char
        }

        //Case 4 : we read a #
        if(str[i] == '#') //End of the line
        {
            str[i] = '\0';
            return TRUE;
        } 
    }
    
    return TRUE;
}



//Retreiving token list from a string
char** retreive_token(char* line, char* const separator)
{
    char** tokens = (char**)malloc(sizeof(char*)*128); //MAX 128 elements per line
    char* token_temp, temp;
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