#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>

#include "u_arithmetic_logic.h"
#include "tools.h"

void addu(CPU *cpu, Instruction inst)
{
    // TODO : overflow à gérer
    cpu->U[inst.destination] = cpu->U[inst.source_1] + cpu->U[inst.source_2];
    cpu->IP += INSTRUCTION_BYTES;
}

void subu(CPU *cpu, Instruction inst)
{
    if (cpu->U[inst.source_2] == 0.0)
    {
        cpu->CF |= 2;
    }
    cpu->U[inst.destination] = cpu->U[inst.source_1] - cpu->U[inst.source_2];
    cpu->IP += INSTRUCTION_BYTES;
}

void mulu(CPU *cpu, Instruction inst)
{
    cpu->U[inst.destination] = cpu->U[inst.source_1] * cpu->U[inst.source_2];
    cpu->IP += INSTRUCTION_BYTES;
}

void divu(CPU *cpu, Instruction inst)
{
    // the second source operand of a division or a modulo instruction is null
    if (cpu->U[inst.source_2] == 0)
    {
        cpu->CF += 1;
        exit(EXIT_FAILURE);
    }

    cpu->U[inst.destination] = cpu->U[inst.source_1] / cpu->U[inst.source_2];
    cpu->IP += INSTRUCTION_BYTES;
}

void modu(CPU *cpu, Instruction inst)
{
    // the second source operand of a division or a modulo instruction is null
    if (cpu->U[inst.source_2] == 0)
    {
        cpu->CF += 1;
        exit(EXIT_FAILURE);
    }
    cpu->U[inst.destination] = cpu->U[inst.source_1] % cpu->U[inst.source_2];
    cpu->IP += INSTRUCTION_BYTES;
}

void fmau(CPU *cpu, Instruction inst)
{
    // TODO : overflow à gerer
    cpu->U[inst.destination] += cpu->U[inst.source_1] * cpu->U[inst.source_2];
    cpu->IP += INSTRUCTION_BYTES;
}

void sqrtu(CPU *cpu, Instruction inst)
{
    cpu->U[inst.destination] = sqrt(cpu->U[inst.source_1]);
    cpu->IP += INSTRUCTION_BYTES;
}

void logu(CPU *cpu, Instruction inst)
{
    if (cpu->U[inst.source_1] == 0)
    {
        exit(EXIT_FAILURE); // mieux gerer :(
    }
    cpu->U[inst.destination] = log(cpu->U[inst.source_1]);
    cpu->IP += INSTRUCTION_BYTES;
}

void incu(CPU *cpu, Instruction inst)
{
    if (cpu->U[inst.destination] < LLONG_MAX)
    {
        cpu->U[inst.destination]++;
    }
    else
    {
        exit(EXIT_FAILURE); // mieux gerer :(
    }
    cpu->IP += INSTRUCTION_BYTES;
}

void decu(CPU *cpu, Instruction inst)
{
    if (cpu->U[inst.destination] > 0)
    {
        cpu->U[inst.destination]--;
    }
    else
    {
        exit(EXIT_FAILURE); // mieux gerer :(
    }
    cpu->IP += INSTRUCTION_BYTES;
}

void andu(CPU *cpu, Instruction inst)
{
    cpu->U[inst.destination] = cpu->U[inst.source_1] & cpu->U[inst.source_2];
    cpu->IP += INSTRUCTION_BYTES;
}

void oru(CPU *cpu, Instruction inst)
{
    cpu->U[inst.destination] = cpu->U[inst.source_1] | cpu->U[inst.source_2];
    cpu->IP += INSTRUCTION_BYTES;
}

void xoru(CPU *cpu, Instruction inst)
{
    cpu->U[inst.destination] = cpu->U[inst.source_1] ^ cpu->U[inst.source_2];
    cpu->IP += INSTRUCTION_BYTES;
}

void shlu(CPU *cpu, Instruction inst)
{
    // TODO : gerer l'overflow du shift
    cpu->U[inst.destination] = cpu->U[inst.source_1] << cpu->U[inst.source_2];
    cpu->IP += INSTRUCTION_BYTES;
}

void shru(CPU *cpu, Instruction inst)
{
    // TODO : gerer l'overflow du shift
    cpu->U[inst.destination] = cpu->U[inst.source_1] >> cpu->U[inst.source_2];
    cpu->IP += INSTRUCTION_BYTES;
}

void rolu(CPU *cpu, Instruction inst)
{
    cpu->S[inst.destination] = cpu->S[inst.source_1] << ~cpu->S[inst.source_2];
    cpu->IP += INSTRUCTION_BYTES;
}

void roru(CPU *cpu, Instruction inst)
{
    cpu->U[inst.destination] = cpu->U[inst.source_1] >> ~cpu->U[inst.source_2];
    cpu->IP += INSTRUCTION_BYTES;
}

void popcntu(CPU *cpu, Instruction inst)
{
    // Hamming weight: using built-in popcount function
    cpu->U[inst.destination] = __builtin_popcountll(cpu->U[inst.source_1]);
    cpu->IP += INSTRUCTION_BYTES;
}

void lmbu(CPU *cpu, Instruction inst)
{
    // Count leading zero bits using built-in clz function
    cpu->U[inst.destination] = __builtin_clzll(cpu->U[inst.source_1]);
    cpu->IP += INSTRUCTION_BYTES;
}
