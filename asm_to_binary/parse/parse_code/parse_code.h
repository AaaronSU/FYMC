#ifndef _PARSE_CODE_H_

#define _PARSE_CODE_H_

#define bool int

// Last element of array must be NULL !!!
bool detect_alias(char** array);
bool correct_alias(char** array);

//Detect if a line is correctly writed
int detect_op_code(char*,char***);
bool correct_op_code(char**,char**,char***);
bool correct_register_name(char*,char**,char***);

// NOTE: We may want to comment the above
bool correct_line(char** line, char*** op_name_list, char*** register_list);

#endif
