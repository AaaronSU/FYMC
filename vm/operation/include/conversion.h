#ifndef CONVERSION_H
#define CONVERSION_H

#include "cpu.h"
#include "type.h"

void cvtus(CPU *cpu, u32 instruction);
void cvtsu(CPU *cpu, u32 instruction);
void cvtuf(CPU *cpu, u32 instruction);
void cvtfu(CPU *cpu, u32 instruction);
void cvtsf(CPU *cpu, u32 instruction);
void cvtfs(CPU *cpu, u32 instruction);



#endif