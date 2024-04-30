#ifndef _PARSE_H_

#define _PARSE_H_

#include "parse_code/parse_code.h"
#include "parse_data/parse_data.h"
#include "../tools/types.h"


// str_out is a buffer of size strlen(str_in) + sizeof(char)
// define it with char str_out [size_without_spaces(str_in, (strlen(str_in) + sizeof(char)), ' ')]
i32 size_without_chara(const char* str,
                       i32 const taille,
                       i32 const chara);

bool alphanumeric(i32);

i32 label_present(char* str,     char** list,
                  i32 list_size, bool remove_label_sign);

i32 parse(char*** in,      i32* sizes, i32 len, i32* data_start, i32* code_start,
          char*** opcodes, i32  len_op,
          char**  labels,  i32* indice_labels,
          char**  requested_labels, i32* indice_req,
          char*** registers,        i32* sizes_registers, i32 len_register);

#endif
