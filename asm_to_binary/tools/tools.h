#ifndef _TOOLS_H_
#define _TOOLS_H_

//Booleans define to make the code more readable
#define bool int
#define TRUE 1
#define FALSE 0

void remove_space(const char* str_in, char* str_out);
bool file_verification(int const,char** const);
char* load_file(char* const);
void print_file(char* const);
void print_tokens_line(char** const);
char** retreive_token(char* line, char const separator);
char*** tokenize(char* const);
void print_tokens_list(char*** const);
bool add_semicolon(char* str);
void free_char3(char*** char3, int size_dim_1, int size_dim_2);
void free_char2(char** char2, int size_dim_1);
#endif
