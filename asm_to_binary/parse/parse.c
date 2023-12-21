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
                if(str[i] == ',') //This will also mark the end of the word
                {
                    str[i] = ' ';
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



// //Retreiving token list from a string
// char** retreive_token(char* line, char* const separator)
// {
//     char** tokens = (char**)malloc(sizeof(char*)*128); //MAX 128 elements per line
//     char* token_temp, temp;
// 	int i = 0; //Used to count tokens
// 	tokens[i] = strtok(line,separator); //Retreiving first token
// 	while(tokens[i] != NULL) //As long as we still have tokens
// 	{
// 		i = i + 1; //Next token count
// 		tokens[i] = strtok(NULL,separator); //Reading next token
// 	}

//     //Returning token list ended by NULL value
//     return tokens;
// }

//Retreiving token list from a string
char** retreive_token(char* line, char const separator)
{
    //MAX 128 elements per line
    char** tokens = (char**)malloc(sizeof(char*)*128);
    for(int i = 0; i < 128; i++)
    {
        //128 char max per token
        tokens[i] = (char*)malloc(sizeof(char)*128);
    }
    //   V       | strlen = 6 CONTRE 9
    //ADD;U2;U3  |
    char* line_temp = line;
    int i = 0;
    while((line_temp = strchr(line,separator)) != NULL)
    {
        //Copy of the string before the caracter we found
        strncpy(tokens[i],line,strlen(line) - strlen(line_temp));
        line = line_temp + 1; //Skipping the seperator
        i = i + 1;
    }

    //NULL means no more separtor, i.e. last token
    tokens[i] = line;
    tokens[i+1] = NULL;

    //If a string has been cutted, we reconstruct it
    i = 0;
    while(tokens[i] != NULL)
    {
        if(strchr(tokens[i],'\"') != NULL && strchr(tokens[i]+1,'\"') == NULL) //i.e opening quote but no closing quote = string cutted
        {
            int j = 1;
            while((strchr(tokens[i+j],'\"') == NULL)) //i.e not closing quote yet
            {
                strcat(tokens[i],";");
                strcat(tokens[i],tokens[i+j]);
                j = j + 1;
            }

            //We found the closing quote, we add it
            strcat(tokens[i],";");
            strcat(tokens[i],tokens[i+j]);

            //Now we need to move the rest of the words
            int k = 1;
            while(tokens[i+j+k] != NULL)
            {
                tokens[i+k] = tokens[i+j+k];
                k = k + 1;
            }
            tokens[i+k] = NULL;
        }
        i = i + 1;
    }

    //Returning token list ended by NULL value
    return tokens;
}




// ------------ Handling parsing and stuf idk ---------------



// Detects if arg is in string AND before any ""
bool is_in_string(char* string, char* arg)
{
    int taille_arg = strlen(arg);
    int taille_string = strlen(string);  // To not get out of range
    bool im_readin_it = FALSE;
    size_t j = 0;
    if (strstr(string, arg))
    {
        if (strchr(string, '"'))
        {
            for (size_t i = 0; i < taille_string; ++i)
            {
                if (im_readin_it)
                {
                    if (string[i] == arg[j])
                    {
                        ++j;
                        if (j == taille_arg) {
                            return TRUE;
                        }
                    }
                    im_readin_it = FALSE;
                    j = 0;
                }
                else if (string[i] == arg[j])
                {
                    im_readin_it = TRUE;
                    ++j;
                }
                else if (string[i] == '"')
                    break;
            }
        }
        else
            return TRUE;
    }
    return FALSE;
}




// Compte le nombre de lignes "utiles" avant la fin du fichier / prochaine section
long long int compter(char* buffer, FILE *in, bool has_data, bool has_code)
{
    // buffer pour lire lignes
    long long int retour = 0;
    while (fgets(buffer, 1024, in)) {  // Tant qu'il y a qq chose dans le fichier

        // Vérifie qu'on ne change pas de section
        if (is_in_string(buffer, "data:"))
        {
            // Pas le droit à deux sections data ni à data après code
            if (has_data || has_code)
                return -1;
            has_data = TRUE;
            return retour;
        }
        if (is_in_string(buffer, "code:"))
        {
            // Pas le droit à deux sections code
            if (has_code)
                return -1;
            has_code = TRUE;
            return retour;
        }

        // Vérifier que la ligne est "utile"
        char* ligne = strtok(buffer, " \t");
        if (ligne)
        {
            if (ligne[0] == '#')
                {continue;}
            else
                {++retour;}
        }
    }
    return retour;
}


// Count the number of lines in the sections so that we can tokenize them
void nb_ligne_section(char* nom, long long int* nb_data, long long int* nb_code) {
    FILE *in = fopen(nom, "r");

    // Si impossible d'ouvrir le fichier
    if (!in) {
        perror("fopen");
        return; // NOTE: crash here?
    }

    // NOTE: On pourrait mettre ce buffer moins gros (une ligne devrait pas faire plus de 100 je pense)
    char buffer[1024];
    bool has_data = FALSE;
    bool has_code = FALSE;
    *nb_data = 0;
    *nb_code = 0;

    if (is_in_string(buffer, "data:"))  // Si data en premier
    {
        has_data = TRUE;
        *nb_data = compter(buffer, in, has_data, has_code);
        if (*nb_data == -1)
            return; // NOTE: Crash here?
        *nb_code = compter(buffer, in, has_data, has_code);
        if (*nb_code == -1)
            return; // NOTE: Crash here?
    }
    else if (is_in_string(buffer, "code:"))  // Si code en premier
    {
        has_code = TRUE;
        *nb_code = compter(buffer, in, has_data, has_code);
        if (*nb_code == -1)
            return; // NOTE: Crash here?
    }
    else // Ni data ni code en premier ?
        return;  // NOTE: Crash here?

    (void)fclose(in);
    return;
}







