#include "move.h"
#include "tools.h"

void mov(CPU *cpu, Instruction instruction)
{
    
}

void movu(CPU *cpu, Instruction instruction)
{
    cpu->U[instruction.destination] = cpu->U[instruction.source_1];
    cpu->IP += SIZE_INSTRUCTION;
}

void movs(CPU *cpu, Instruction instruction)
{
    
    cpu->S[instruction.destination] = cpu->S[instruction.source_1];
    cpu->IP += SIZE_INSTRUCTION;
}

void movf(CPU *cpu, Instruction instruction)
{
    
    cpu->F[instruction.destination] = cpu->F[instruction.source_1];
    cpu->IP += SIZE_INSTRUCTION;
}

void movv(CPU *cpu, Instruction instruction)
{
    

    for(int i = 0; i < SIZE_VECTOR; ++i) 
    {
        cpu->V[instruction.destination][i] = cpu->V[instruction.source_1][i];
    }
    
    cpu->IP += SIZE_INSTRUCTION;
}

void movt(CPU *cpu, Instruction instruction)
{
    
    
    for(int i = 0; i < SIZE_VECTOR; ++i) 
    {
        cpu->T[instruction.destination][i] = cpu->T[instruction.source_1][i];
    }
    cpu->IP += SIZE_INSTRUCTION;
}

void movg(CPU *cpu, Instruction instruction) 
{
    
    
    for(int i = 0; i < SIZE_VECTOR; ++i) 
    {
        cpu->G[instruction.destination][i] = cpu->G[instruction.source_1][i];
    }
    cpu->IP += SIZE_INSTRUCTION;
}

void movui(CPU *cpu, Instruction instruction) 
{
    
    u64 immediat = 0;
    cpu->U[instruction.destination] = immediat;
    cpu->IP += SIZE_INSTRUCTION + 64;
}

void movsi(CPU *cpu, Instruction instruction) 
{
    
    u64 immediat = (i64)0;
    cpu->S[instruction.destination] = immediat;
    cpu->IP += SIZE_INSTRUCTION + 64;
}

void movfi(CPU *cpu, Instruction instruction) 
{
    
    u64 immediat = (f64)0;
    cpu->F[instruction.destination] = immediat;
    cpu->IP += SIZE_INSTRUCTION + 64;
}

void movvi(CPU *cpu, Instruction instruction) 
{
    
    u64 immediat[8]; // pour l'instant

    for(int i = 0; i < SIZE_VECTOR; ++i) 
    {
        cpu->V[instruction.destination][i] = immediat[i];
    }
    cpu->IP += SIZE_INSTRUCTION + 64 * 8;
}

void movti(CPU *cpu, Instruction instruction)
{
    
    u64 immediat[8]; // pour l'instant

    for(int i = 0; i < SIZE_VECTOR; ++i) 
    {
        cpu->T[instruction.destination][i] = (i64)immediat[i];
    }
    cpu->IP += SIZE_INSTRUCTION + 64 * 8;
}

void movgi(CPU *cpu, Instruction instruction)
{
    
    u64 immediat[8]; // pour l'instant

    for(int i = 0; i < SIZE_VECTOR; ++i) 
    {
        cpu->G[instruction.destination][i] = (f64)immediat[i];
    }
    cpu->IP += SIZE_INSTRUCTION + 64 * 8;
}