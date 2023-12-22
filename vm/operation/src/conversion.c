#include "conversion.h"
#include "tools.c"

#include <limits.h>

void cvtus(CPU *cpu, Instruction inst)
{
    cpu->S[inst.destination] = (i64)cpu->U[inst.source_1];
    cpu->IP += INSTRUCTION_BYTES;
}

void cvtsu(CPU *cpu, Instruction inst)
{
    cpu->U[inst.destination] = (u64)cpu->S[inst.source_1];
    cpu->IP += INSTRUCTION_BYTES;
}

void cvtuf(CPU *cpu, Instruction inst)
{
    cpu->F[inst.destination] = (f64)cpu->U[inst.source_1];
    cpu->IP += INSTRUCTION_BYTES;
}

void cvtfu(CPU *cpu, Instruction inst)
{
    cpu->U[inst.destination] = (u64)cpu->F[inst.source_1];
    cpu->IP += INSTRUCTION_BYTES;
}

void cvtsf(CPU *cpu, Instruction inst)
{
    cpu->F[inst.destination] = (f64)cpu->S[inst.source_1];
    cpu->IP += INSTRUCTION_BYTES;
}

void cvtfs(CPU *cpu, Instruction inst)
{
    cpu->S[inst.destination] = (i64)cpu->F[inst.source_1];
    cpu->IP += INSTRUCTION_BYTES;
}