#include <stdlib.h>
#include <math.h>
#include "s_arithmetic_logic.h"
#include "cpu.h"
#include "tools.h"

void adds(CPU *cpu, Instruction inst)
{
    cpu->S[inst.destination] = cpu->S[inst.source_1] + cpu->S[inst.source_2];
    cpu->IP += INSTRUCTION_BYTES;
}

void subs(CPU *cpu, Instruction inst)
{
    cpu->S[inst.destination] = cpu->S[inst.source_1] - cpu->S[inst.source_2];
    cpu->IP += INSTRUCTION_BYTES;
}

void muls(CPU *cpu, Instruction inst)
{
    cpu->S[inst.destination] = cpu->S[inst.source_1] * cpu->S[inst.source_2];
    cpu->IP += INSTRUCTION_BYTES;
}

void divs(CPU *cpu, Instruction inst)
{
    if (cpu->S[inst.source_2] == 0.0)
    {
        cpu->CF |= 2;
    }
    cpu->S[inst.destination] = cpu->S[inst.source_1] / cpu->S[inst.source_2];
    cpu->IP += INSTRUCTION_BYTES;
}

void mods(CPU *cpu, Instruction inst)
{
    if (cpu->S[inst.source_2] == 0)
    {
        cpu->CF += 1; // Erreur : modulo par zéro
        exit(EXIT_FAILURE);
    }
    cpu->S[inst.destination] = cpu->S[inst.source_1] % cpu->S[inst.source_2];
    cpu->IP += INSTRUCTION_BYTES;
}

void fmas(CPU *cpu, Instruction inst)
{
    // TODO : gestion de l'overflow

    cpu->S[inst.destination] += cpu->S[inst.source_1] * cpu->S[inst.source_2];
    cpu->IP += INSTRUCTION_BYTES;
}

void sqrts(CPU *cpu, Instruction inst)
{
    if (cpu->S[inst.source_1] < 0)
    {
        exit(EXIT_FAILURE);
    }
    cpu->S[inst.destination] = sqrt(cpu->S[inst.source_1]);
    cpu->IP += INSTRUCTION_BYTES;
}

void logs(CPU *cpu, Instruction inst)
{
    if (cpu->S[inst.source_1] <= 0)
    {
        exit(EXIT_FAILURE);
    }
    cpu->S[inst.destination] = log(cpu->S[inst.source_1]);
    cpu->IP += INSTRUCTION_BYTES;
}

void ands(CPU *cpu, Instruction inst)
{
    cpu->S[inst.destination] = cpu->S[inst.source_1] & cpu->S[inst.source_2];
    cpu->IP += INSTRUCTION_BYTES;
}

void ors(CPU *cpu, Instruction inst)
{
    cpu->S[inst.destination] = cpu->S[inst.source_1] | cpu->S[inst.source_2];
    cpu->IP += INSTRUCTION_BYTES;
}

void xors(CPU *cpu, Instruction inst)
{
    cpu->S[inst.destination] = cpu->S[inst.source_1] ^ cpu->S[inst.source_2];
    cpu->IP += INSTRUCTION_BYTES;
}

void shls(CPU *cpu, Instruction inst)
{
    // TODO : gestion de l'overflow

    cpu->S[inst.destination] = cpu->S[inst.source_1] << cpu->S[inst.source_2];
    cpu->IP += INSTRUCTION_BYTES;
}

void shrs(CPU *cpu, Instruction inst)
{
    // TODO : gestion de l'overflow

    cpu->S[inst.destination] = cpu->S[inst.source_1] >> cpu->S[inst.source_2];
    cpu->IP += INSTRUCTION_BYTES;
}

void rols(CPU *cpu, Instruction inst)
{
    cpu->S[inst.destination] = cpu->S[inst.source_1] << ~cpu->S[inst.source_2];
    cpu->IP += INSTRUCTION_BYTES;
}

void rors(CPU *cpu, Instruction inst)
{
    cpu->S[inst.destination] = cpu->S[inst.source_1] >> ~cpu->S[inst.source_2];
    cpu->IP += INSTRUCTION_BYTES;
}

void popcnts(CPU *cpu, Instruction inst)
{
    cpu->U[inst.destination] = __builtin_popcountll(cpu->U[inst.source_1]);
    cpu->IP += INSTRUCTION_BYTES;
}

void lmbs(CPU *cpu, Instruction inst)
{
    cpu->U[inst.destination] = __builtin_clzll(cpu->U[inst.source_1]);
    cpu->IP += INSTRUCTION_BYTES;
}
