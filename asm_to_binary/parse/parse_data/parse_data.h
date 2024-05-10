#ifndef _PARSE_DATA_H_

#define _PARSE_DATA_H_

#include "../../tools/types.h"

bool wrong_signing_symbol(char* str);
bool good_integer        (char* str, bool is_signed);
bool good_float          (char* str);
bool good_variable       (char** line);

#endif
