#include <limits.h>

#include "mov.h"
#include "tools.h"

void mov(CPU *cpu, u32 instruction);

void movu(CPU *cpu, u32 instruction)
{
    Instruction inst = parse_instruction(instruction);

    if (cpu->U[inst.source_1] <= LLONG_MAX) 
    {
        cpu->S[inst.destination] = (i64)cpu->U[inst.source_1];
    } 
    else 
    {
        // faut que je gère l'erreur : maybe erno  ou jsp
    }
    cpu->IP += SIZE_INSTRUCTION;
}

void movs(CPU *cpu, u32 instruction)
{
    Instruction inst = parse_instruction(instruction);
    cpu->S[inst.destination] = cpu->S[inst.source_1];
    cpu->IP += SIZE_INSTRUCTION;
}

void movf(CPU *cpu, u32 instruction)
{
    Instruction inst = parse_instruction(instruction);
    cpu->F[inst.destination] = cpu->F[inst.source_1];
    cpu->IP += SIZE_INSTRUCTION;
}

void movv(CPU *cpu, u32 instruction)
{
    Instruction inst = parse_instruction(instruction);

    for(int i = 0; i < SIZE_VECTOR; ++i) 
    {
        cpu->V[inst.destination][i] = cpu->V[inst.source_1][i];
    }
    
    cpu->IP += SIZE_INSTRUCTION;
}

void movt(CPU *cpu, u32 instruction)
{
    Instruction inst = parse_instruction(instruction);
    
    for(int i = 0; i < SIZE_VECTOR; ++i) 
    {
        cpu->T[inst.destination][i] = cpu->T[inst.source_1][i];
    }
    cpu->IP += SIZE_INSTRUCTION;
}

void movg(CPU *cpu, u32 instruction) 
{
    Instruction inst = parse_instruction(instruction);
    
    for(int i = 0; i < SIZE_VECTOR; ++i) 
    {
        cpu->G[inst.destination][i] = cpu->G[inst.source_1][i];
    }
    cpu->IP += SIZE_INSTRUCTION;
}

void movui(CPU *cpu, u32 instruction, u64 immediat) 
{
    Instruction inst = parse_instruction(instruction);
    cpu->U[inst.destination] = immediat;
    cpu->IP += SIZE_INSTRUCTION + 64;
}

void movsi(CPU *cpu, u32 instruction, i64 immediat) 
{
    Instruction inst = parse_instruction(instruction);
    cpu->S[inst.destination] = immediat;
    cpu->IP += SIZE_INSTRUCTION + 64;
}

void movfi(CPU *cpu, u32 instruction, f64 immediat) 
{
    Instruction inst = parse_instruction(instruction);
    cpu->F[inst.destination] = immediat;
    cpu->IP += SIZE_INSTRUCTION + 64;
}

void movvi(CPU *cpu, u32 instruction, u64 immediat[]) 
{
    Instruction inst = parse_instruction(instruction);

    for(int i = 0; i < SIZE_VECTOR; ++i) 
    {
        cpu->V[inst.destination][i] = immediat[i];
    }
    cpu->IP += SIZE_INSTRUCTION + 64 * 8;
}

void movti(CPU *cpu, u32 instruction, i64 immediat[])
{
    Instruction inst = parse_instruction(instruction);

    for(int i = 0; i < SIZE_VECTOR; ++i) 
    {
        cpu->T[inst.destination][i] = immediat[i];
    }
    cpu->IP += SIZE_INSTRUCTION + 64 * 8;
}

void movgi(CPU *cpu, u32 instruction, f64 immediat[])
{
    Instruction inst = parse_instruction(instruction);

    for(int i = 0; i < SIZE_VECTOR; ++i) 
    {
        cpu->G[inst.destination][i] = immediat[i];
    }
    cpu->IP += SIZE_INSTRUCTION + 64 * 8;
}