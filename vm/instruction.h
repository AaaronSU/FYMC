#include <stdlib.h>
#include <string.h>
#include "vm.h"

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

extern void (*instruction_set[MAX_INSTRUCTION_NUMBER])(core_t *);

extern void loadu(core_t *core);
extern void loads(core_t *core);
extern void loadf(core_t *core);
extern void loadv(core_t *core);
extern void loadt(core_t *core);
extern void loadg(core_t *core);
extern void storeu(core_t *core);
extern void stores(core_t *core);
extern void storef(core_t *core);
extern void storev(core_t *core);
extern void storet(core_t *core);
extern void storeg(core_t *core);
extern void mov(core_t *core);
extern void movu(core_t *core);
extern void movs(core_t *core);
extern void movf(core_t *core);
extern void movv(core_t *core);
extern void movt(core_t *core);
extern void movg(core_t *core);
extern void movui(core_t *core);
extern void movsi(core_t *core);
extern void movfi(core_t *core);
extern void movvi(core_t *core);
extern void movti(core_t *core);
extern void movgi(core_t *core);
extern void cvtus(core_t *core);
extern void cvtsu(core_t *core);
extern void cvtuf(core_t *core);
extern void cvtfu(core_t *core);
extern void cvtsf(core_t *core);
extern void cvtfs(core_t *core);
extern void addu(core_t *core);
extern void subu(core_t *core);
extern void mulu(core_t *core);
extern void divu(core_t *core);
extern void modu(core_t *core);
extern void fmau(core_t *core);
extern void sqrtu(core_t *core);
extern void logu(core_t *core);
extern void incu(core_t *core);
extern void decu(core_t *core);
extern void andu(core_t *core);
extern void oru(core_t *core);
extern void xoru(core_t *core);
extern void shlu(core_t *core);
extern void shru(core_t *core);
extern void rolu(core_t *core);
extern void roru(core_t *core);
extern void popcntu(core_t *core);
extern void lmbu(core_t *core);
extern void adds(core_t *core);
extern void subs(core_t *core);
extern void muls(core_t *core);
extern void divs(core_t *core);
extern void mods(core_t *core);
extern void fmas(core_t *core);
extern void sqrts(core_t *core);
extern void logs(core_t *core);
extern void ands(core_t *core);
extern void ors(core_t *core);
extern void xors(core_t *core);
extern void shls(core_t *core);
extern void shrs(core_t *core);
extern void rols(core_t *core);
extern void rors(core_t *core);
extern void popcnts(core_t *core);
extern void lmbs(core_t *core);
extern void addf(core_t *core);
extern void subf(core_t *core);
extern void mulf(core_t *core);
extern void divf(core_t *core);
extern void fmaf_(core_t *core);
extern void sqrtf_(core_t *core);
extern void logf_(core_t *core);
extern void cmpu(core_t *core);
extern void cmps(core_t *core);
extern void cmpf(core_t *core);
extern void je(core_t *core);
extern void jne(core_t *core);
extern void jge(core_t *core);
extern void jl(core_t *core);
extern void jle(core_t *core);
extern void jz(core_t *core);
extern void jnz(core_t *core);
extern void outu(core_t *core);
extern void outs(core_t *core);
extern void outf(core_t *core);
extern void outa(core_t *core);
extern void outb(core_t *core);
extern void outx(core_t *core);
extern void hlt(core_t *core);
extern void mulg(core_t *core);
extern void addg(core_t *core);
extern void outg(core_t *core);
extern void sumg(core_t *core);
extern void fmag(core_t *core);

extern void undefined_instruction(core_t *core);

extern void set_up_instruction_set();

#endif