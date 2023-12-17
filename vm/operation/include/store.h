#ifndef STORE_H
#define STORE_H

#include "cpu.h"
#include "type.h"

void storeu(CPU *cpu, u32 instruction);
void stores(CPU *cpu, u32 instruction);
void storef(CPU *cpu, u32 instruction);
void storev(CPU *cpu, u32 instruction);
void storet(CPU *cpu, u32 instruction);
void storeg(CPU *cpu, u32 instruction);

#endif