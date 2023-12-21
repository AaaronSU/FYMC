#ifndef U_ARITHM_LOGIC_H
#define U_ARITHM_LOGIC_H
// Unsigned arithmetic and logic

#include "cpu.h"

void addu(CPU *cpu, u32 instruction);    // U0 = U1 + U2
void subu(CPU *cpu, u32 instruction);    // U0 = U1 - U2
void mulu(CPU *cpu, u32 instruction);    // U0 = U1 * U2
void divu(CPU *cpu, u32 instruction);    // U0 = U1 / U2
void modu(CPU *cpu, u32 instruction);    // U0 = U1 % U2
void fmau(CPU *cpu, u32 instruction);    // U0 = U0 + U1 * U2
void sqrtu(CPU *cpu, u32 instruction);   // U0 = sqrtu(U1)
void logu(CPU *cpu, u32 instruction);    // U0 = logu(U1)
void incu(CPU *cpu, u32 instruction);    // U0 = U0 + 1
void decu(CPU *cpu, u32 instruction);    // U0 = U0 - 1
void andu(CPU *cpu, u32 instruction);    // U0 = U1 and U2
void oru(CPU *cpu, u32 instruction);     // U0 = U1 or U2
void xoru(CPU *cpu, u32 instruction);    // U0 = U1 xor U2
void shlu(CPU *cpu, u32 instruction);    // U0 = U1 << U2
void shru(CPU *cpu, u32 instruction);    // U0 = U1 >> U2
void rolu(CPU *cpu, u32 instruction);    // U0 = U1 ~<< U2
void roru(CPU *cpu, u32 instruction);    // U0 = U1 >>~ U2
void popcntu(CPU *cpu, u32 instruction); // U0 = popcount(U1)
void lmbu(CPU *cpu, u32 instruction);    // U0 = leftmostbit(U1)

#endif