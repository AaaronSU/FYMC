#ifndef LOAD_H
#define LOAD_H

#include "cpu.h"

void loadu(CPU *cpu, u32 instruction);
void loads(CPU *cpu, u32 instruction);
void loadf(CPU *cpu, u32 instruction);
void loadv(CPU *cpu, u32 instruction);
void loadt(CPU *cpu, u32 instruction);
void loadg(CPU *cpu, u32 instruction);

#endif