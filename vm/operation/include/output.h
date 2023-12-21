#ifndef OUTPUT_H
#define OUTPUT_H

#include "cpu.h"
#include "tools.h"
#include "type.h"

void outu(CPU *cpu, u32 instruction);
void outs(CPU *cpu, u32 instruction);
void outf(CPU *cpu, u32 instruction);

void outa(CPU *cpu, u32 instruction);
void outb(CPU *cpu, u32 instruction);
void outx(CPU *cpu, u32 instruction);
#endif