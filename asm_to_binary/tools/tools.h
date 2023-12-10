#ifndef _TOOLS_H_
#define _TOOLS_H_

//Booleans define to make the code more readable
#define bool int
#define TRUE 1
#define FALSE 0

bool file_verification(int,char**);
char* load_file(char*);
bool load_op_code(char*, int*);
void print_file(char*);
void print_tokens(char** const, char const);

#endif