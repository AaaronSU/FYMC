#ifndef _PARSE_CODE_H_

#define _PARSE_CODE_H_

// str_out is a buffer of size sizeof(char) * ((strlen(str_in) + 2))
// define it with char str_out[sizeof(char) * ((strlen(str_in) + 2))]
void add_comma(const char* str_in, char* str_out);

// str_out is a buffer of size strlen(str_in) + sizeof(char)
// define it with char str_out [size_without_spaces(str_in, (strlen(str_in) + sizeof(char)), ' ')]
int size_without_chara(const char* str,
                        int const taille,
                        int const chara);
void remove_space(const char* str_in, char* str_out);


char** retreive_token(char* const line, char* const separator);

#endif