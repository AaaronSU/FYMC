#ifndef LOAD_H
#define LOAD_H

#include "cpu.h"

void loadu(CPU *cpu, Instruction instruction);
void loads(CPU *cpu, Instruction instruction);
void loadf(CPU *cpu, Instruction instruction);
void loadv(CPU *cpu, Instruction instruction);
void loadt(CPU *cpu, Instruction instruction);
void loadg(CPU *cpu, Instruction instruction);

#endif