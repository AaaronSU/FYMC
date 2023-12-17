#include "load.h"
#include "tools.h"

#include <stdio.h>
#include <stdlib.h>

void loadu(CPU *cpu, u32 instruction) 
{
    // TODO : overflow 
    Instruction inst = parse_instruction(instruction);
    u8 offset = get_offset(inst.unused);
    u64 addr = cpu->U[inst.source_1] + cpu->U[inst.source_2] + offset;
    if (addr >= MEMORY_SIZE) 
    { 
        printf("Dépassement de mémoire\n");
        exit(EXIT_FAILURE); // pour l'instant
    }

    cpu->U[inst.destination] = cpu->Memory[addr];
    cpu->IP += SIZE_INSTRUCTION;
}

void loads(CPU *cpu, u32 instruction)
{
    // TODO : gérer le dépassement de 1 bit
    Instruction inst = parse_instruction(instruction);
    u8 offset = get_offset(inst.unused);
    u64 addr = cpu->U[inst.source_1] + cpu->U[inst.source_2] + offset;

    if (addr >= MEMORY_SIZE) 
    { 
        printf("Dépassement de mémoire\n");
        exit(EXIT_FAILURE); // pour l'instant
    }

    cpu->S[inst.destination] = cpu->Memory[addr];
    cpu->IP += SIZE_INSTRUCTION;
}


void loadf(CPU *cpu, u32 instruction) 
{
    // TODO : gérer le dépassement de 1 bit
    Instruction inst = parse_instruction(instruction);
    u8 offset = get_offset(inst.unused);
    u64 addr = cpu->U[inst.source_1] + cpu->U[inst.source_2] + offset;

    if (addr >= MEMORY_SIZE) 
    { 
        printf("Dépassement de mémoire\n");
        exit(EXIT_FAILURE); // pour l'instant
    }

    cpu->F[inst.destination] = cpu->Memory[addr];
    cpu->IP += SIZE_INSTRUCTION;
}

void loadv(CPU *cpu, u32 instruction) 
{
    // TODO : gérer le dépassement de 1 bit
    Instruction inst = parse_instruction(instruction);
    u8 offset = get_offset(inst.unused);
    u64 addr = cpu->U[inst.source_1] + cpu->U[inst.source_2] + offset;

    if (addr >= MEMORY_SIZE) 
    { 
        printf("Dépassement de mémoire\n");
        exit(EXIT_FAILURE); // pour l'instant
    }

    for(int i = 0; i <SIZE_VECTOR; ++i) 
    {
        cpu->V[inst.destination][i] = cpu->Memory[addr + i];
    }
    cpu->IP += SIZE_INSTRUCTION;
}

void loadt(CPU *cpu, u32 instruction) 
{
    // TODO : gérer le dépassement de 1 bit
    Instruction inst = parse_instruction(instruction);
    u8 offset = get_offset(inst.unused);
    u64 addr = cpu->U[inst.source_1] + cpu->U[inst.source_2] + offset;

    if (addr >= MEMORY_SIZE) 
    { 
        printf("Dépassement de mémoire\n");
        exit(EXIT_FAILURE); // pour l'instant
    }

    for(int i = 0; i <SIZE_VECTOR; ++i) 
    {
        cpu->T[inst.destination][i] = cpu->Memory[addr + i];
    }
    cpu->IP += SIZE_INSTRUCTION;
}

void loadg(CPU *cpu, u32 instruction)
{
    // TODO : gérer le dépassement de 1 bit
    Instruction inst = parse_instruction(instruction);
    u8 offset = get_offset(inst.unused);
    u64 addr = cpu->U[inst.source_1] + cpu->U[inst.source_2] + offset;

    if (addr >= MEMORY_SIZE) 
    { 
        printf("Dépassement de mémoire\n");
        exit(EXIT_FAILURE); // pour l'instant
    }

    for(int i = 0; i <SIZE_VECTOR; ++i) 
    {
        cpu->G[inst.destination][i] = cpu->Memory[addr + i];
    }
    cpu->IP += SIZE_INSTRUCTION;
}