#ifndef _TOOLS_H_
#define _TOOLS_H_

//Booleans define to make the code more readable
#define bool int
#define TRUE 1
#define FALSE 0

bool file_verification(int const,char** const);
char* load_file(char* const);
void print_file(char* const);
void print_tokens_line(char** const);
void tokenize(char* const,char***);
void print_tokens_list(char*** const);
void free_char3(char*** char3, int size_dim_1, int size_dim_2);
void free_char2(char** char2, int size_dim_1);
#endif
