#ifndef _ASSEMBLY_H_
#define _ASSEMBLY_H_

//Booleans define to make the code more readable
#define bool int
#define TRUE 1
#define FALSE 0

void write_stuff(char* path, char*** data_array, char*** code_array,
                long long int* const nb_data, long long int* const nb_code,
                char*** op_name_list, char*** register_list, char** address_array);

#endif
