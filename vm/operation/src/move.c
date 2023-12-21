#include "move.h"
#include "tools.h"
#include <stdio.h>

void mov(CPU *cpu, u32 instruction) {}

void movu(CPU *cpu, u32 instruction)
{
    Instruction inst = parse_instruction(instruction);

    cpu->U[inst.destination] = cpu->U[inst.source_1];
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

    for (int i = 0; i < SIZE_VECTOR; ++i)
    {
        cpu->V[inst.destination][i] = cpu->V[inst.source_1][i];
    }

    cpu->IP += SIZE_INSTRUCTION;
}

void movt(CPU *cpu, u32 instruction)
{
    Instruction inst = parse_instruction(instruction);

    for (int i = 0; i < SIZE_VECTOR; ++i)
    {
        cpu->T[inst.destination][i] = cpu->T[inst.source_1][i];
    }
    cpu->IP += SIZE_INSTRUCTION;
}

void movg(CPU *cpu, u32 instruction)
{
    Instruction inst = parse_instruction(instruction);

    for (int i = 0; i < SIZE_VECTOR; ++i)
    {
        cpu->G[inst.destination][i] = cpu->G[inst.source_1][i];
    }
    cpu->IP += SIZE_INSTRUCTION;
}

void movui(CPU *cpu, u32 instruction)
{
    Instruction inst = parse_instruction(instruction);
    cpu->IP += 4;

    int i = 0;
    int immediat = 0;
    while (i < 8)
    {
        char *value_adress = &(cpu->BinaryFile[cpu->IP]);
        int value = (int)*value_adress;
        immediat += value << (8 - 1);
        printf("value: %d, immediat= %d\n", value, immediat);
        i++;
        cpu->IP += 1;
    }

    // cpu->IP += 8;
    cpu->U[inst.destination] = immediat;
    printf("mov %d dans U[%d]", immediat, inst.destination);
}

void movsi(CPU *cpu, u32 instruction)
{
    Instruction inst = parse_instruction(instruction);

    u64 immediat = (i64)0;
    cpu->S[inst.destination] = immediat;
    cpu->IP += SIZE_INSTRUCTION + 64;
}

void movfi(CPU *cpu, u32 instruction)
{
    Instruction inst = parse_instruction(instruction);

    u64 immediat = (f64)0;
    cpu->F[inst.destination] = immediat;
    cpu->IP += SIZE_INSTRUCTION + 64;
}

void movvi(CPU *cpu, u32 instruction)
{
    Instruction inst = parse_instruction(instruction);

    u64 immediat[8]; // pour l'instant

    for (int i = 0; i < SIZE_VECTOR; ++i)
    {
        cpu->V[inst.destination][i] = immediat[i];
    }
    cpu->IP += SIZE_INSTRUCTION + 64 * 8;
}

void movti(CPU *cpu, u32 instruction)
{
    Instruction inst = parse_instruction(instruction);

    u64 immediat[8]; // pour l'instant

    for (int i = 0; i < SIZE_VECTOR; ++i)
    {
        cpu->T[inst.destination][i] = (i64)immediat[i];
    }
    cpu->IP += SIZE_INSTRUCTION + 64 * 8;
}

void movgi(CPU *cpu, u32 instruction)
{
    Instruction inst = parse_instruction(instruction);

    u64 immediat[8]; // pour l'instant

    for (int i = 0; i < SIZE_VECTOR; ++i)
    {
        cpu->G[inst.destination][i] = (f64)immediat[i];
    }
    cpu->IP += SIZE_INSTRUCTION + 64 * 8;
}