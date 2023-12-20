#ifndef MOV_H
#define MOV_H

#include "cpu.h"
#include "type.h"

void mov(CPU *cpu, Instruction instruction);
void movu(CPU *cpu, Instruction instruction);
void movs(CPU *cpu, Instruction instruction);
void movf(CPU *cpu, Instruction instruction);
void movv(CPU *cpu, Instruction instruction);
void movt(CPU *cpu, Instruction instruction);
void movg(CPU *cpu, Instruction instruction);

void movui(CPU *cpu, Instruction instruction);
void movsi(CPU *cpu, Instruction instruction);
void movfi(CPU *cpu, Instruction instruction);

void movvi(CPU *cpu, Instruction instruction);
void movti(CPU *cpu, Instruction instruction);
void movgi(CPU *cpu, Instruction instruction);

#endif