#ifndef S_ARTH_LOGIC_H
#define S_ARTH_LOGIC_H

//  Signed arithmetic and logic

#include "cpu.h"

void adds(CPU *cpu, Instruction instruction);
void subs(CPU *cpu, Instruction instruction);
void muls(CPU *cpu, Instruction instruction);
void divs(CPU *cpu, Instruction instruction);
void mods(CPU *cpu, Instruction instruction);
void fmas(CPU *cpu, Instruction instruction);
void sqrts(CPU *cpu, Instruction instruction);
void logs(CPU *cpu, Instruction instruction);
void ands(CPU *cpu, Instruction instruction);
void ors(CPU *cpu, Instruction instruction);
void xors(CPU *cpu, Instruction instruction);
void shls(CPU *cpu, Instruction instruction);
void shrs(CPU *cpu, Instruction instruction);
void rols(CPU *cpu, Instruction instruction);
void rors(CPU *cpu, Instruction instruction);
void popcnts(CPU *cpu, Instruction instruction);
void lmbs(CPU *cpu, Instruction instruction);

#endif