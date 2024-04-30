#include "f_arithmetic_logic.h"
#include <stdlib.h>
#include <math.h>
#include "cpu.h"
#include "tools.h"

void addf(CPU *cpu, Instruction inst)
{
    cpu->F[inst.destination] = cpu->F[inst.source_1] + cpu->F[inst.source_2];
    cpu->IP += INSTRUCTION_BYTES;
}

void subf(CPU *cpu, Instruction inst)
{
    cpu->F[inst.destination] = cpu->F[inst.source_1] - cpu->F[inst.source_2];
    cpu->IP += INSTRUCTION_BYTES;
}

void mulf(CPU *cpu, Instruction inst)
{
    cpu->F[inst.destination] = cpu->F[inst.source_1] * cpu->F[inst.source_2];
    cpu->IP += INSTRUCTION_BYTES;
}

void divf(CPU *cpu, Instruction inst)
{
    if (cpu->F[inst.source_2] == 0.0)
    {
        cpu->CF |= 2;
    }
    cpu->F[inst.destination] = cpu->F[inst.source_1] / cpu->F[inst.source_2];
    cpu->IP += INSTRUCTION_BYTES;
}

void fmaf_(CPU *cpu, Instruction inst)
{
    cpu->F[inst.destination] += cpu->F[inst.source_1] * cpu->F[inst.source_2];
    cpu->IP += INSTRUCTION_BYTES;
}

void sqrtf_(CPU *cpu, Instruction inst)
{
    if (cpu->F[inst.source_1] < 0.0)
    {
        cpu->CF |= 2;
    }
    cpu->F[inst.destination] = sqrt(cpu->F[inst.source_1]);
    cpu->IP += INSTRUCTION_BYTES;
}

void logf_(CPU *cpu, Instruction inst)
{
    if (cpu->F[inst.source_1] <= 0.0)
    {
        cpu->CF |= 2;
    }
    cpu->F[inst.destination] = log(cpu->F[inst.source_1]);
    cpu->IP += INSTRUCTION_BYTES;
}
