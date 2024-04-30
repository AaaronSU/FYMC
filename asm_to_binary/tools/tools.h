#ifndef _TOOLS_H_
#define _TOOLS_H_

#include <stdlib.h>
#include <stdio.h>
#include "types.h"

void remove_space     (const char* str_in, char* str_out);
bool file_verification(i32 const,          char** const);
char* load_file(char* const);
void print_file(char* const);
void print_tokens_line(char** const, i32);
char** retreive_token (char* line,   char const separator);
i32 make_tokens(char*, char**,  u64);
i32 tokenize   (FILE*, char***, i32*);
void print_tokens_list(char*** const, i32*, i32);
void free_char3(char*** char3, i32 size_dim_1, i32 size_dim_2);
void free_char2(char**  char2, i32 size_dim_1);
#endif
