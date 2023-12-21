#include "conversion.h"
#include "tools.c"

#include <limits.h>

void cvtus(CPU *cpu, Instruction inst)
{
    // S0, U0
    if (cpu->U[inst.source_1] <= LLONG_MAX)
    {
        cpu->S[inst.destination] = (i64)cpu->U[inst.source_1];
    }
    else
    {
        // faut gerer l'erreur de conversion -> a voir après
    }
    cpu->IP += SIZE_INSTRUCTION;
}

void cvtsu(CPU *cpu, Instruction inst)
{
    if (cpu->S[inst.source_1] >= 0)
    {
        cpu->U[inst.destination] = (u64)cpu->S[inst.source_1];
    }
    else
    {
        // à voir
    }
    cpu->IP += SIZE_INSTRUCTION;
}

void cvtuf(CPU *cpu, Instruction inst)
{
    cpu->F[inst.destination] = (f64)cpu->U[inst.source_1];
    cpu->IP += SIZE_INSTRUCTION;
}

void cvtfu(CPU *cpu, Instruction inst)
{
    cpu->U[inst.destination] = (u64)cpu->F[inst.source_1];
    cpu->IP += SIZE_INSTRUCTION;
}

void cvtsf(CPU *cpu, Instruction inst)
{
    cpu->F[inst.destination] = (f64)cpu->S[inst.source_1];
    cpu->IP += SIZE_INSTRUCTION;
}

void cvtfs(CPU *cpu, Instruction inst)
{
    cpu->S[inst.destination] = (i64)cpu->F[inst.source_1];
    cpu->IP += SIZE_INSTRUCTION;
}