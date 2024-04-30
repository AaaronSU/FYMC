#ifndef F_ARITHM_LOGIC_H
#define F_ARITHM_LOGIC_H

#include "cpu.h"

void addf(CPU *cpu, Instruction instruction);
void subf(CPU *cpu, Instruction instruction);
void mulf(CPU *cpu, Instruction instruction);
void divf(CPU *cpu, Instruction instruction);
void fmaf_(CPU *cpu, Instruction instruction);
void sqrtf_(CPU *cpu, Instruction instruction);
void logf_(CPU *cpu, Instruction instruction);

#endif