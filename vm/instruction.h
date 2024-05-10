#include <stdlib.h>
#include <string.h>
#include "vm.h"

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

extern void (*instruction_set[MAX_INSTRUCTION_NUMBER])(core_t *);

void loadu(core_t *core);
void storeu(core_t *core);
void movu(core_t *core);
void movui(core_t *core);
void addu(core_t *core);
void mulu(core_t *core);
void fmau(core_t *core);
void incu(core_t *core);
void cmpu(core_t *core);
void jl(core_t *core);
void outu(core_t *core);
void outb(core_t *core);
void hlt(core_t *core);
void set_up_instruction_set();

#endif