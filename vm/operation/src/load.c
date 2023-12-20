#include "load.h"
#include "tools.h"

#include <stdio.h>
#include <stdlib.h>

void loadu(CPU *cpu, Instruction instruction) 
{
    // TODO : overflow 
    
    u8 offset = get_offset(instruction.unused);
    u64 addr = cpu->U[instruction.source_1] + cpu->U[instruction.source_2] + offset;
    if (addr >= MEMORY_SIZE) 
    { 
        printf("Dépassement de mémoire\n");
        exit(EXIT_FAILURE); // pour l'instant
    }

    cpu->U[instruction.destination] = cpu->Memory[addr];
    cpu->IP += SIZE_INSTRUCTION;
}

void loads(CPU *cpu, Instruction instruction)
{
    // TODO : gérer le dépassement de 1 bit
    
    u8 offset = get_offset(instruction.unused);
    u64 addr = cpu->U[instruction.source_1] + cpu->U[instruction.source_2] + offset;

    if (addr >= MEMORY_SIZE) 
    { 
        printf("Dépassement de mémoire\n");
        exit(EXIT_FAILURE); // pour l'instant
    }

    cpu->S[instruction.destination] = cpu->Memory[addr];
    cpu->IP += SIZE_INSTRUCTION;
}


void loadf(CPU *cpu, Instruction instruction) 
{
    // TODO : gérer le dépassement de 1 bit
    
    u8 offset = get_offset(instruction.unused);
    u64 addr = cpu->U[instruction.source_1] + cpu->U[instruction.source_2] + offset;

    if (addr >= MEMORY_SIZE) 
    { 
        printf("Dépassement de mémoire\n");
        exit(EXIT_FAILURE); // pour l'instant
    }

    cpu->F[instruction.destination] = cpu->Memory[addr];
    cpu->IP += SIZE_INSTRUCTION;
}

void loadv(CPU *cpu, Instruction instruction) 
{
    // TODO : gérer le dépassement de 1 bit
    
    u8 offset = get_offset(instruction.unused);
    u64 addr = cpu->U[instruction.source_1] + cpu->U[instruction.source_2] + offset;

    if (addr >= MEMORY_SIZE) 
    { 
        printf("Dépassement de mémoire\n");
        exit(EXIT_FAILURE); // pour l'instant
    }

    for(int i = 0; i <SIZE_VECTOR; ++i) 
    {
        cpu->V[instruction.destination][i] = cpu->Memory[addr + i];
    }
    cpu->IP += SIZE_INSTRUCTION;
}

void loadt(CPU *cpu, Instruction instruction) 
{
    // TODO : gérer le dépassement de 1 bit
    
    u8 offset = get_offset(instruction.unused);
    u64 addr = cpu->U[instruction.source_1] + cpu->U[instruction.source_2] + offset;

    if (addr >= MEMORY_SIZE) 
    { 
        printf("Dépassement de mémoire\n");
        exit(EXIT_FAILURE); // pour l'instant
    }

    for(int i = 0; i <SIZE_VECTOR; ++i) 
    {
        cpu->T[instruction.destination][i] = cpu->Memory[addr + i];
    }
    cpu->IP += SIZE_INSTRUCTION;
}

void loadg(CPU *cpu, Instruction instruction)
{
    // TODO : gérer le dépassement de 1 bit
    
    u8 offset = get_offset(instruction.unused);
    u64 addr = cpu->U[instruction.source_1] + cpu->U[instruction.source_2] + offset;

    if (addr >= MEMORY_SIZE) 
    { 
        printf("Dépassement de mémoire\n");
        exit(EXIT_FAILURE); // pour l'instant
    }

    for(int i = 0; i <SIZE_VECTOR; ++i) 
    {
        cpu->G[instruction.destination][i] = cpu->Memory[addr + i];
    }
    cpu->IP += SIZE_INSTRUCTION;
}