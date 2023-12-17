#ifndef MOV_H
#define MOV_H

#include "cpu.h"
#include "type.h"

void mov(CPU *cpu, u32 instruction);
void movu(CPU *cpu, u32 instruction);
void movs(CPU *cpu, u32 instruction);
void movf(CPU *cpu, u32 instruction);
void movv(CPU *cpu, u32 instruction);
void movt(CPU *cpu, u32 instruction);
void movg(CPU *cpu, u32 instruction);

void movui(CPU *cpu, u32 instruction, u64 immediat);
void movsi(CPU *cpu, u32 instruction, i64 immediat);
void movfi(CPU *cpu, u32 instruction, f64 immediat);

void movvi(CPU *cpu, u32 instruction, u64 immediat[]);
void movti(CPU *cpu, u32 instruction, i64 immediat[]);
void movgi(CPU *cpu, u32 instruction, f64 immediat[]);

#endif