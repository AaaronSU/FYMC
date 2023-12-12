#ifndef _TOOLS_H_
#define _TOOLS_H_

//Booleans define to make the code more readable
#define bool int
#define TRUE 1
#define FALSE 0

bool file_verification(int const,char** const);
char* load_file(char* const);
void print_file(char* const);
void print_tokens_line(char** const, char const);
char*** tokenize(char* const);
void print_tokens_list(char*** const, char const);
#endif