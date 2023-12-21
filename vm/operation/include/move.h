#ifndef MOVE_H
#define MOVE_H

#include "cpu.h"
#include "type.h"

void mov(CPU *cpu, u32 instruction);
void movu(CPU *cpu, u32 instruction);
void movs(CPU *cpu, u32 instruction);
void movf(CPU *cpu, u32 instruction);
void movv(CPU *cpu, u32 instruction);
void movt(CPU *cpu, u32 instruction);
void movg(CPU *cpu, u32 instruction);

void movui(CPU *cpu, u32 instruction);
void movsi(CPU *cpu, u32 instruction);
void movfi(CPU *cpu, u32 instruction);

void movvi(CPU *cpu, u32 instruction);
void movti(CPU *cpu, u32 instruction);
void movgi(CPU *cpu, u32 instruction);

#endif