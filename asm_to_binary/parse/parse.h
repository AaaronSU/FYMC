#ifndef _PARSE_H_

#define _PARSE_H_

#include "parse_code/parse_code.h"
#include "parse_data/parse_data.h"


// str_out is a buffer of size strlen(str_in) + sizeof(char)
// define it with char str_out [size_without_spaces(str_in, (strlen(str_in) + sizeof(char)), ' ')]
int size_without_chara(const char* str,
                        int const taille,
                        int const chara);

void remove_space(const char* str_in, char* str_out);
bool add_semicolon(char*);
char** retreive_token(char* const line, char const separator);

bool alphanumeric(int);


void nb_ligne_section(char* const nom, long long int* nb_data, long long int* nb_code);

// TODO: test function (normal cases, empty sections, secctions of size 1 and other weird cases)
bool parse(char* const nom, char*** data_array, char*** code_array,
           long long int* const nb_data, long long int* const nb_code,
           char*** op_name_list, char*** register_list);

#endif
