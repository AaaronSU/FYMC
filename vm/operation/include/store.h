#ifndef STORE_H
#define STORE_H

#include "cpu.h"
#include "type.h"

void storeu(CPU *cpu, Instruction instruction);
void stores(CPU *cpu, Instruction instruction);
void storef(CPU *cpu, Instruction instruction);
void storev(CPU *cpu, Instruction instruction);
void storet(CPU *cpu, Instruction instruction);
void storeg(CPU *cpu, Instruction instruction);

#endif