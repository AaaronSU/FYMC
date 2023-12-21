#include "store.h"
#include "tools.h"

#include <stdlib.h>
#include <stdio.h>

// stores a scalar unsigned value (orSIZE_VECTOR ASCII characters) in U0 into the memory located at U0 + U1 + OFFSET.
void storeu(CPU *cpu, u32 instruction)
{
    // ça doit donc enregistre la valeur de u2 dans la mémoire à l'adresse u0 + u1 + offset
    // TODO : gérer le dépassement de 1 bit + vérifier l'adresse existante ?
    // (U0, U1 [, OFFSET]), U2
    Instruction inst = parse_instruction(instruction);

    u8 offset = get_offset(inst.unused);
    u64 addr = cpu->U[inst.destination] + cpu->U[inst.source_1] + offset; //  (U0, U1 [, OFFSET])

    if (addr >= MEMORY_SIZE)
    {
        printf("Dépassement de mémoire\n");
        exit(EXIT_FAILURE); // pour l'instant
    }

    cpu->Memory[addr] = cpu->U[inst.destination]; // Memory[addr] = U2;
    cpu->IP += SIZE_INSTRUCTION;
};
void stores(CPU *cpu, u32 instruction)
{
    // ça doit donc enregistre la valeur de u2 dans la mémoire à l'adresse u0 + u1 + offset
    // TODO : gérer le dépassement de 1 bit + vérifier l'adresse existante ?
    // (U0, U1 [, OFFSET]), U2
    Instruction inst = parse_instruction(instruction);

    u8 offset = get_offset(inst.unused);
    u64 addr = cpu->U[inst.destination] + cpu->U[inst.source_1] + offset; //  (U0, U1 [, OFFSET])

    if (addr >= MEMORY_SIZE)
    {
        printf("Dépassement de mémoire\n");
        exit(EXIT_FAILURE); // pour l'instant
    }

    cpu->Memory[addr] = cpu->S[inst.destination]; // Memory[addr] = U2;
    cpu->IP += SIZE_INSTRUCTION;
};
void storef(CPU *cpu, u32 instruction)
{
    // ça doit donc enregistre la valeur de u2 dans la mémoire à l'adresse u0 + u1 + offset
    // TODO : gérer le dépassement de 1 bit + vérifier l'adresse existante ?
    // (U0, U1 [, OFFSET]), U2
    Instruction inst = parse_instruction(instruction);

    u8 offset = get_offset(inst.unused);
    u64 addr = cpu->U[inst.destination] + cpu->U[inst.source_1] + offset; //  (U0, U1 [, OFFSET])

    if (addr >= MEMORY_SIZE)
    {
        printf("Dépassement de mémoire\n");
        exit(EXIT_FAILURE); // pour l'instant
    }

    cpu->Memory[addr] = cpu->F[inst.destination]; // Memory[addr] = U2;
    cpu->IP += SIZE_INSTRUCTION;
};
void storev(CPU *cpu, u32 instruction)
{
    Instruction inst = parse_instruction(instruction);

    u8 offset = get_offset(inst.unused);
    u64 addr = cpu->U[inst.destination] + cpu->U[inst.source_1] + offset;

    if (addr >= MEMORY_SIZE)
    {
        printf("Dépassement de mémoire\n");
        exit(EXIT_FAILURE); // pour l'instant
    }

    for (int i = 0; i < SIZE_VECTOR; ++i)
    {
        cpu->Memory[addr + i] = cpu->V[inst.destination][i];
    }
    cpu->IP += SIZE_INSTRUCTION;
};
void storet(CPU *cpu, u32 instruction)
{
    Instruction inst = parse_instruction(instruction);

    u8 offset = get_offset(inst.unused);
    u64 addr = cpu->U[inst.destination] + cpu->U[inst.source_1] + offset;

    if (addr >= MEMORY_SIZE)
    {
        printf("Dépassement de mémoire\n");
        exit(EXIT_FAILURE); // pour l'instant
    }

    for (int i = 0; i < SIZE_VECTOR; ++i)
    {
        cpu->Memory[addr + i] = cpu->T[inst.destination][i];
    }
    cpu->IP += SIZE_INSTRUCTION;
};
void storeg(CPU *cpu, u32 instruction)
{
    Instruction inst = parse_instruction(instruction);
    u8 offset = get_offset(inst.unused);
    u64 addr = cpu->U[inst.destination] + cpu->U[inst.source_1] + offset;

    if (addr >= MEMORY_SIZE)
    {
        printf("Dépassement de mémoire\n");
        exit(EXIT_FAILURE); // pour l'instant
    }

    for (int i = 0; i < SIZE_VECTOR; ++i)
    {
        cpu->Memory[addr + i] = cpu->G[inst.destination][i];
    }
    cpu->IP += SIZE_INSTRUCTION;
};