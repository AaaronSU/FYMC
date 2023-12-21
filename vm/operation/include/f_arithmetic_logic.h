#ifndef F_ARITHM_LOGIC_H
#define F_ARITHM_LOGIC_H

#include "cpu.h"

void addf(CPU *cpu, u32 opcode);
void subf(CPU *cpu, u32 opcode);
void mulf(CPU *cpu, u32 opcode);
void divf(CPU *cpu, u32 opcode);
void fmaf_(CPU *cpu, u32 opcode);
void sqrtf_(CPU *cpu, u32 opcode);
void logf_(CPU *cpu, u32 opcode);

#endif