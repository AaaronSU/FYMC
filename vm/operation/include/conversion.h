#ifndef CONVERSION_H
#define CONVERSION_H

#include "cpu.h"

void cvtus(CPU *cpu, Instruction instruction);
void cvtsu(CPU *cpu, Instruction instruction);
void cvtuf(CPU *cpu, Instruction instruction);
void cvtfu(CPU *cpu, Instruction instruction);
void cvtsf(CPU *cpu, Instruction instruction);
void cvtfs(CPU *cpu, Instruction instruction);

#endif