#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "parse_code/parse_code.h"
#include "parse_data/parse_data.h"
#include "parse.h"
#include "../tools/tools.h"



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
        if(isalpha(str[i]) || isdigit(str[i]))
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

            ++i;
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
            if (ligne[0] == '#' || ligne[0] == '\n')
                {continue;}
            else
                {++retour;}
        }
    }
    return retour;
}


// Count the number of lines in the sections so that we can tokenize them
void nb_ligne_section(char* const nom, long long int* nb_data, long long int* nb_code) {
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

    while (fgets(buffer, 1024, in))
    {
        if (is_in_string(buffer, "data:"))  // Si data en premier
        {
            has_data = TRUE;
            *nb_data = compter(buffer, in, has_data, has_code);
            if (*nb_data == -1)
                {
                    (void)fclose(in);
                    return; // NOTE: Crash here?
                }
            *nb_code = compter(buffer, in, has_data, has_code);
            if (*nb_code == -1)
                {
                    (void)fclose(in);
                    return; // NOTE: Crash here?
                }
            break;
        }
        else if (is_in_string(buffer, "code:"))  // Si code en premier
        {
            has_code = TRUE;
            // TODO: test empty data section
            *nb_data = 0;
            *nb_code = compter(buffer, in, has_data, has_code);
            if (*nb_code == -1)
                {
                    (void)fclose(in);
                    return; // NOTE: Crash here?
                }
            break;
        }
        else // Ni data ni code en premier ?
            {
                char string_tmp[1024];
                remove_space(buffer, string_tmp);
                if (buffer[0] == '#' || buffer[0] == '\n')
                    continue;
                // Set to avoid problems
                *nb_code = -1;
                *nb_data = -1;
                (void)fclose(in);
                return;  // NOTE: Crash here?
            }
    }

    (void)fclose(in);
    return;
}




// Devrait parser le code
// nom -> nom du fichier à parser
// data_array et code_array sont le retour de la fonction (les malloc/calloc de nb_data/nb_code)
// nb_data et nb_code construits via nb_ligne_section
// op_name_list et register_list => voir main.c
bool parse(char* const nom, char*** data_array, char*** code_array,
           long long int* const nb_data, long long int* const nb_code,
           char*** op_name_list, char*** register_list)
{
    if (*nb_code == -1)
        {
            // Actually that shouldn't be reachable
            return FALSE;
        }  // code and / or data sections not placed correctly

    FILE *file = fopen(nom, "r");
    bool all_good = TRUE;

    // Si impossible d'ouvrir le fichier
    if (!file) {
        perror("fopen");
        return FALSE; // NOTE: crash here?
    }


    long long int indice_data = 0;
    long long int indice_code = 0;
    char ligne[1024];
    char* char_ptr_tmp = calloc(1024, sizeof(char));


    // Handling data section
    if (*nb_data > 0)
    {
        // Move towards data section
        char* char_ptr = fgets(ligne, 1024, file);
        remove_space(char_ptr, char_ptr_tmp);
        while (strcmp(char_ptr_tmp, "data:") != 0)
        {
            char_ptr = fgets(ligne, 1024, file);
            remove_space(char_ptr, char_ptr_tmp);
        }

        for (size_t i = 0; i < *nb_data; ++i)
        {

            char_ptr = fgets(ligne, 1024, file);
            if (is_in_string(char_ptr, "#"))
                { --i; continue;}
            add_semicolon(char_ptr);
            remove_space(char_ptr, char_ptr_tmp);
            if (char_ptr_tmp[0] == '\n')
                    {--i; continue;}

            char** token_thing = retreive_token(char_ptr_tmp, ';');
            if (is_valid(token_thing) == FALSE)
                {
                    all_good = FALSE;
                    free_char2(token_thing, 128);
                    break;
                }
            size_t k = 0;
            while (token_thing[k] != NULL)
            {
                // On fait des strcpy car on veut pas garder token_thing en vie
                strcpy(data_array[indice_data][k], token_thing[k]);
                ++k;
            }
            free(data_array[indice_data][k]);
            data_array[indice_data][k] = NULL;
            ++indice_data;

            free_char2(token_thing, 128);
        }
    }

    if (all_good)
    {
        // Handling code section
        // Move towards code section
        char* char_ptr = fgets(ligne, 1024, file);
        remove_space(char_ptr, char_ptr_tmp);
        while (strcmp(char_ptr_tmp, "code:") != 0)
        {
            char_ptr = fgets(ligne, 1024, file);
            remove_space(char_ptr, char_ptr_tmp);
        }

        for (size_t i = 0; i < *nb_code; ++i)
        {
            char_ptr = fgets(ligne, 1024, file);
            if (is_in_string(char_ptr, "#"))
                {--i; continue;}

            add_semicolon(char_ptr);
            remove_space(char_ptr, char_ptr_tmp);
            if (char_ptr_tmp[0] == '\n')
                {--i; continue;}

            char** token_thing = retreive_token(char_ptr_tmp, ';');
            if (correct_line(token_thing, op_name_list, register_list) == FALSE)
                {
                    all_good = FALSE;
                    free_char2(token_thing, 128);
                    break;
                }
            size_t k = 0;
            while (token_thing[k] != NULL)
            {
                strcpy(code_array[indice_code][k], token_thing[k]);
                ++k;
            }
            free(code_array[indice_code][k]);
            code_array[indice_code][k] = NULL;
            ++indice_code;

            free_char2(token_thing, 128);
        }

    }

    (void)fclose(file);
    free(char_ptr_tmp);
    free(code_array[*nb_code]);
    code_array[*nb_code] = NULL;

    if (*nb_data > 0)
    {
        free(data_array[*nb_data]);
        data_array[*nb_data] = NULL;
    }

    return all_good;
}
