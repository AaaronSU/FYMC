#ifndef TOOLS_H
#define TOOLS_H

#include "type.h"
#include "cpu.h"


Instruction parse_instruction(u32 instruction);
u8 get_offset(u16 unused);

#endif