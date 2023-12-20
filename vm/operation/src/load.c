#include "load.h"
#include "tools.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * Loads a value from memory into an unsigned register.
 * 
 * @param cpu Pointer to the CPU structure.
 * @param instruction Structure containing details of the instruction (e.g., source, destination).
 */
void loadu(CPU *cpu, Instruction instruction) 
{
    u8 offset = get_offset(instruction.unused);
    u64 addr = cpu->U[instruction.source_1] + cpu->U[instruction.source_2] + offset;
    if (addr >= MEMORY_SIZE) 
    { 
        printf("Dépassement de mémoire\n");
        exit(EXIT_FAILURE);
    }

    cpu->U[instruction.destination] = cpu->Memory[addr];
    cpu->IP += SIZE_INSTRUCTION;
}

/**
 * Loads a value from memory into a signed register.
 * 
 * @param cpu Pointer to the CPU structure.
 * @param instruction Structure containing details of the instruction (e.g., source, destination).
 */
void loads(CPU *cpu, Instruction instruction)
{
    u8 offset = get_offset(instruction.unused);
    u64 addr = cpu->U[instruction.source_1] + cpu->U[instruction.source_2] + offset;

    if (addr >= MEMORY_SIZE) 
    { 
        printf("Dépassement de mémoire\n");
        exit(EXIT_FAILURE);
    }

    cpu->S[instruction.destination] = cpu->Memory[addr];
    cpu->IP += SIZE_INSTRUCTION;
}

/**
 * Loads a value from memory into a floating-point register.
 * 
 * @param cpu Pointer to the CPU structure.
 * @param instruction Structure containing details of the instruction (e.g., source, destination).
 */
void loadf(CPU *cpu, Instruction instruction) 
{
    u8 offset = get_offset(instruction.unused);
    u64 addr = cpu->U[instruction.source_1] + cpu->U[instruction.source_2] + offset;

    if (addr >= MEMORY_SIZE) 
    { 
        printf("Dépassement de mémoire\n");
        exit(EXIT_FAILURE);
    }

    cpu->F[instruction.destination] = cpu->Memory[addr];
    cpu->IP += SIZE_INSTRUCTION;
}

/**
 * Loads a value from memory into an unsigned vector register.
 * 
 * @param cpu Pointer to the CPU structure.
 * @param instruction Structure containing details of the instruction (e.g., source, destination).
 */
void loadv(CPU *cpu, Instruction instruction) 
{
    u8 offset = get_offset(instruction.unused);
    u64 addr = cpu->U[instruction.source_1] + cpu->U[instruction.source_2] + offset;

    if (addr >= MEMORY_SIZE) 
    { 
        printf("Dépassement de mémoire\n");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i <SIZE_VECTOR; ++i) 
    {
        cpu->V[instruction.destination][i] = cpu->Memory[addr + i];
    }
    cpu->IP += SIZE_INSTRUCTION;
}

/**
 * Loads a value from memory into an signed vector register.
 * 
 * @param cpu Pointer to the CPU structure.
 * @param instruction Structure containing details of the instruction (e.g., source, destination).
 */
void loadt(CPU *cpu, Instruction instruction) 
{ 
    u8 offset = get_offset(instruction.unused);
    u64 addr = cpu->U[instruction.source_1] + cpu->U[instruction.source_2] + offset;

    if (addr >= MEMORY_SIZE) 
    { 
        printf("Dépassement de mémoire\n");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i <SIZE_VECTOR; ++i) 
    {
        cpu->T[instruction.destination][i] = cpu->Memory[addr + i];
    }
    cpu->IP += SIZE_INSTRUCTION;
}

/**
 * Loads a value from memory into an floating-point vector register.
 * 
 * @param cpu Pointer to the CPU structure.
 * @param instruction Structure containing details of the instruction (e.g., source, destination).
 */
void loadg(CPU *cpu, Instruction instruction)
{
    u8 offset = get_offset(instruction.unused);
    u64 addr = cpu->U[instruction.source_1] + cpu->U[instruction.source_2] + offset;

    if (addr >= MEMORY_SIZE) 
    { 
        printf("Dépassement de mémoire\n");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i <SIZE_VECTOR; ++i) 
    {
        cpu->G[instruction.destination][i] = cpu->Memory[addr + i];
    }
    cpu->IP += SIZE_INSTRUCTION;
}