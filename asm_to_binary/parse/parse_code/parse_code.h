#ifndef _PARSE_CODE_H_

#define _PARSE_CODE_H_

#include "../../tools/types.h"

i32 good_opcode   (char* in, char*** opcodes,   i32 len_op);
bool good_register(char* in, char*** registers, i32 len_registers,
                   char letter);
i32 label_present (char* str, char** list, i32 list_size,
                   bool remove_label_sign);

#endif
