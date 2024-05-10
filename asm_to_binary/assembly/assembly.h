#ifndef _ASSEMBLY_H_
#define _ASSEMBLY_H_

#include "../tools/types.h"

void write_stuff(char*     path,
                 char***   tokens_list,   i32* tokens_list_sizes, i32 nb_tokens,
                 i32 const data_start,    i32 const code_start,
                 char***   op_name_list,  i32 op_size,
                 char***   register_list, i32 register_size,
                 char**    address_array, i32  address_size);

#endif
