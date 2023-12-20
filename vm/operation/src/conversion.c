#include "conversion.h"
#include "tools.c"

#include <limits.h>

void cvtus(CPU *cpu, Instruction instruction) 
{
    // S0, U0
    if (cpu->U[instruction.source_1] <= LLONG_MAX) 
    {
        cpu->S[instruction.destination] = (i64)cpu->U[instruction.source_1];
    } else {
        // faut gerer l'erreur de conversion -> a voir après
    }
    cpu->IP += SIZE_INSTRUCTION;
}

void cvtsu(CPU *cpu, Instruction instruction)
{
    
    if (cpu->S[instruction.source_1] >= 0)
    {
        cpu->U[instruction.destination] = (u64)cpu->S[instruction.source_1];
    } 
    else 
    {
        // à voir 
    }
    cpu->IP += SIZE_INSTRUCTION;
}

void cvtuf(CPU *cpu, Instruction instruction)
{
    
    cpu->F[instruction.destination] = (f64)cpu->U[instruction.source_1];
    cpu->IP += SIZE_INSTRUCTION;
}

void cvtfu(CPU *cpu, Instruction instruction)
{
    
    cpu->U[instruction.destination] = (u64)cpu->F[instruction.source_1];
    cpu->IP += SIZE_INSTRUCTION;
}

void cvtsf(CPU *cpu, Instruction instruction)
{
    
    cpu->F[instruction.destination] = (f64)cpu->S[instruction.source_1];
    cpu->IP += SIZE_INSTRUCTION;
}

void cvtfs(CPU *cpu, Instruction instruction)
{
    
    cpu->S[instruction.destination] = (i64)cpu->F[instruction.source_1];
    cpu->IP += SIZE_INSTRUCTION;
}