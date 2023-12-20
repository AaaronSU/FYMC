#include "store.h"
#include "tools.h"

#include <stdlib.h>
#include <stdio.h>

// stores a scalar unsigned value (orSIZE_VECTOR ASCII characters) in U0 into the memory located at U0 + U1 + OFFSET.
void storeu(CPU *cpu, Instruction instruction)
{
    // ça doit donc enregistre la valeur de u2 dans la mémoire à l'adresse u0 + u1 + offset
    // TODO : gérer le dépassement de 1 bit + vérifier l'adresse existante ? 
    // (U0, U1 [, OFFSET]), U2
    
    u8 offset = get_offset(instruction.unused);
    u64 addr = cpu->U[instruction.destination] + cpu->U[instruction.source_1] + offset; //  (U0, U1 [, OFFSET])

    if (addr >= MEMORY_SIZE) 
    { 
        printf("Dépassement de mémoire\n");
        exit(EXIT_FAILURE); // pour l'instant
    }

    cpu->Memory[addr] = cpu->U[instruction.destination]; // Memory[addr] = U2;
    cpu->IP += SIZE_INSTRUCTION;


};
void stores(CPU *cpu, Instruction instruction)
{
    // ça doit donc enregistre la valeur de u2 dans la mémoire à l'adresse u0 + u1 + offset
    // TODO : gérer le dépassement de 1 bit + vérifier l'adresse existante ? 
    // (U0, U1 [, OFFSET]), U2
    
    u8 offset = get_offset(instruction.unused);
    u64 addr = cpu->U[instruction.destination] + cpu->U[instruction.source_1] + offset; //  (U0, U1 [, OFFSET])

    if (addr >= MEMORY_SIZE) 
    { 
        printf("Dépassement de mémoire\n");
        exit(EXIT_FAILURE); // pour l'instant
    }

    cpu->Memory[addr] = cpu->S[instruction.destination]; // Memory[addr] = U2;
    cpu->IP += SIZE_INSTRUCTION;
};
void storef(CPU *cpu, Instruction instruction)
{
    // ça doit donc enregistre la valeur de u2 dans la mémoire à l'adresse u0 + u1 + offset
    // TODO : gérer le dépassement de 1 bit + vérifier l'adresse existante ? 
    // (U0, U1 [, OFFSET]), U2
    
    u8 offset = get_offset(instruction.unused);
    u64 addr = cpu->U[instruction.destination] + cpu->U[instruction.source_1] + offset; //  (U0, U1 [, OFFSET])

    if (addr >= MEMORY_SIZE) 
    { 
        printf("Dépassement de mémoire\n");
        exit(EXIT_FAILURE); // pour l'instant
    }

    cpu->Memory[addr] = cpu->F[instruction.destination]; // Memory[addr] = U2;
    cpu->IP += SIZE_INSTRUCTION;
};
void storev(CPU *cpu, Instruction instruction)
{
    
    u8 offset = get_offset(instruction.unused);
    u64 addr = cpu->U[instruction.destination] + cpu->U[instruction.source_1] + offset;

    if (addr >= MEMORY_SIZE) 
    { 
        printf("Dépassement de mémoire\n");
        exit(EXIT_FAILURE); // pour l'instant
    }

    for(int i = 0; i <SIZE_VECTOR; ++i) 
    {
        cpu->Memory[addr + i] = cpu->V[instruction.destination][i];
    }
    cpu->IP += SIZE_INSTRUCTION;
};
void storet(CPU *cpu, Instruction instruction)
{
    
    u8 offset = get_offset(instruction.unused);
    u64 addr = cpu->U[instruction.destination] + cpu->U[instruction.source_1] + offset;

    if (addr >= MEMORY_SIZE) 
    { 
        printf("Dépassement de mémoire\n");
        exit(EXIT_FAILURE); // pour l'instant
    }

    for(int i = 0; i <SIZE_VECTOR; ++i) 
    {
        cpu->Memory[addr + i] = cpu->T[instruction.destination][i];
    }
    cpu->IP += SIZE_INSTRUCTION;
};
void storeg(CPU *cpu, Instruction instruction) 
{
    
    u8 offset = get_offset(instruction.unused);
    u64 addr = cpu->U[instruction.destination] + cpu->U[instruction.source_1] + offset;

    if (addr >= MEMORY_SIZE) 
    { 
        printf("Dépassement de mémoire\n");
        exit(EXIT_FAILURE); // pour l'instant
    }

    for(int i = 0; i <SIZE_VECTOR; ++i) 
    {
        cpu->Memory[addr + i] = cpu->G[instruction.destination][i];
    }
    cpu->IP += SIZE_INSTRUCTION;
};