#ifndef _PARSE_DATA_H_

#define _PARSE_DATA_H_

//Booleans define to make the code more readable
//Might be defined somewhere else so might have to remove it from here
#define bool int
#define TRUE 1
#define FALSE 0


bool seems_valid(char** array);
bool type_exists(char* str);
bool name_is_valid(char* str);

#endif
