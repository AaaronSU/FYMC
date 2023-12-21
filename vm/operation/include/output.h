#ifndef OUTPUT_H
#define OUTPUT_H

#include "cpu.h"
#include "tools.h"
#include "type.h"

void outu(CPU *cpu, Instruction instruction);
void outs(CPU *cpu, Instruction instruction);
void outf(CPU *cpu, Instruction instruction);

void outa(CPU *cpu, Instruction instruction);
void outb(CPU *cpu, Instruction instruction);
void outx(CPU *cpu, Instruction instruction);
#endif