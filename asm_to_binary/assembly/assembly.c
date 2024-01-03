#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "../parse/parse_code/parse_code.h"
#include "../parse/parse_data/parse_data.h"
#include "../parse/parse.h"
#include "../tools/tools.h"


void write_stuff(char* path, char*** data_array, char*** code_array,
                long long int* const nb_data, long long int* const nb_code,
                char*** op_name_list, char*** register_list)
{
  // print_tokens_list(data_array);
  // print_tokens_list(code_array);
  // printf("%lld\n", *nb_code);
}
