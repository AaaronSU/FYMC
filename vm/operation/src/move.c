#include "move.h"
#include "tools.h"
#include <stdio.h>

void mov(CPU *cpu, Instruction inst)
{
    (void)cpu;
    (void)inst;
}

void movu(CPU *cpu, Instruction inst)
{
    cpu->IP += INSTRUCTION_BYTES;
    cpu->U[inst.destination] = cpu->U[inst.source_1];
}

void movs(CPU *cpu, Instruction inst)
{
    cpu->IP += INSTRUCTION_BYTES;
    cpu->S[inst.destination] = cpu->S[inst.source_1];
}

void movf(CPU *cpu, Instruction inst)
{
    cpu->IP += INSTRUCTION_BYTES;
    cpu->F[inst.destination] = cpu->F[inst.source_1];
}

void movv(CPU *cpu, Instruction inst)
{
    cpu->IP += INSTRUCTION_BYTES;
    for (int i = 0; i < SIZE_VECTOR; ++i)
    {
        cpu->V[inst.destination][i] = cpu->V[inst.source_1][i];
    }
}

void movt(CPU *cpu, Instruction inst)
{
    cpu->IP += INSTRUCTION_BYTES;
    for (int i = 0; i < SIZE_VECTOR; ++i)
    {
        cpu->T[inst.destination][i] = cpu->T[inst.source_1][i];
    }
}

void movg(CPU *cpu, Instruction inst)
{
    cpu->IP += INSTRUCTION_BYTES;
    for (int i = 0; i < SIZE_VECTOR; ++i)
    {
        cpu->G[inst.destination][i] = cpu->G[inst.source_1][i];
    }
}

void movui(CPU *cpu, Instruction inst)
{
    cpu->IP += INSTRUCTION_BYTES;
    int immediat = get_immediate(cpu);
    printf("instruction destination : %d\n", inst.destination);
    cpu->U[inst.destination] = immediat;
}

void movsi(CPU *cpu, Instruction inst)
{
    cpu->IP += INSTRUCTION_BYTES;
    int immediat = get_immediate(cpu);
    cpu->S[inst.destination] = immediat;
}

void movfi(CPU *cpu, Instruction inst)
{
    cpu->IP += INSTRUCTION_BYTES;
    int immediat = get_immediate(cpu);
    cpu->F[inst.destination] = immediat;
}

void movvi(CPU *cpu, Instruction inst)
{
    cpu->IP += INSTRUCTION_BYTES;
    for (int i = 0; i < SIZE_VECTOR; ++i)
    {
        cpu->V[inst.destination][i] = get_immediate(cpu);
    }
}

void movti(CPU *cpu, Instruction inst)
{
    cpu->IP += INSTRUCTION_BYTES;
    for (int i = 0; i < SIZE_VECTOR; ++i)
    {
        cpu->T[inst.destination][i] = get_immediate(cpu);
    }
}

void movgi(CPU *cpu, Instruction inst)
{
    cpu->IP += INSTRUCTION_BYTES;
    for (int i = 0; i < SIZE_VECTOR; ++i)
    {
        cpu->G[inst.destination][i] = get_immediate(cpu);
    }
}