#ifndef S_ARTH_LOGIC_H
#define S_ARTH_LOGIC_H

//  Signed arithmetic and logic

#include "cpu.h"

void adds(CPU *cpu, u32 instruction);
void subs(CPU *cpu, u32 instruction);
void muls(CPU *cpu, u32 instruction);
void divs(CPU *cpu, u32 instruction);
void mods(CPU *cpu, u32 instruction);
void fmas(CPU *cpu, u32 instruction);
void sqrts(CPU *cpu, u32 instruction);
void logs(CPU *cpu, u32 instruction);
void ands(CPU *cpu, u32 instruction);
void ors(CPU *cpu, u32 instruction);
void xors(CPU *cpu, u32 instruction);
void shls(CPU *cpu, u32 instruction);
void shrs(CPU *cpu, u32 instruction);
void rols(CPU *cpu, u32 instruction);
void rors(CPU *cpu, u32 instruction);
void popcnts(CPU *cpu, u32 instruction);
void lmbs(CPU *cpu, u32 instruction);

#endif