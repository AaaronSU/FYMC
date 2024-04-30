#ifndef COMPARE_JUMP_H
#define COMPARE_JUMP_H

#include "cpu.h"

void cmpu(CPU *cpu, Instruction instruction);
void cmps(CPU *cpu, Instruction instruction);
void cmpf(CPU *cpu, Instruction instruction);
void je(CPU *cpu, Instruction instruction);
void jne(CPU *cpu, Instruction instruction);
void jge(CPU *cpu, Instruction instruction);
void jl(CPU *cpu, Instruction instruction);
void jle(CPU *cpu, Instruction instruction);
void jz(CPU *cpu, Instruction instruction);
void jnz(CPU *cpu, Instruction instruction);

#endif