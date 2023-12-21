#include <stdlib.h>
#include <math.h>
#include "s_arithmetic_logic.h"
#include "cpu.h"
#include "tools.h"

void adds(CPU *cpu, Instruction inst)
{
    // TODO : gestion de l'overflow
    cpu->S[inst.destination] = cpu->S[inst.source_1] + cpu->S[inst.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void subs(CPU *cpu, Instruction inst)
{
    // TODO : gestion de l'overflow
    cpu->S[inst.destination] = cpu->S[inst.source_1] - cpu->S[inst.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void muls(CPU *cpu, Instruction inst)
{
    // TODO : gestion de l'overflow
    cpu->S[inst.destination] = cpu->S[inst.source_1] * cpu->S[inst.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void divs(CPU *cpu, Instruction inst)
{
    if (cpu->S[inst.source_2] == 0)
    {
        cpu->CF += 1; // Erreur : division par zéro
        exit(EXIT_FAILURE);
    }
    cpu->S[inst.destination] = cpu->S[inst.source_1] / cpu->S[inst.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void mods(CPU *cpu, Instruction inst)
{
    if (cpu->S[inst.source_2] == 0)
    {
        cpu->CF += 1; // Erreur : modulo par zéro
        exit(EXIT_FAILURE);
    }
    cpu->S[inst.destination] = cpu->S[inst.source_1] % cpu->S[inst.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void fmas(CPU *cpu, Instruction inst)
{
    // TODO : gestion de l'overflow

    cpu->S[inst.destination] += cpu->S[inst.source_1] * cpu->S[inst.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void sqrts(CPU *cpu, Instruction inst)
{
    if (cpu->S[inst.source_1] < 0)
    {
        exit(EXIT_FAILURE);
    }
    cpu->S[inst.destination] = sqrt(cpu->S[inst.source_1]);
    cpu->IP += SIZE_INSTRUCTION;
}

void logs(CPU *cpu, Instruction inst)
{
    if (cpu->S[inst.source_1] <= 0)
    {
        exit(EXIT_FAILURE);
    }
    cpu->S[inst.destination] = log(cpu->S[inst.source_1]);
    cpu->IP += SIZE_INSTRUCTION;
}

void ands(CPU *cpu, Instruction inst)
{
    cpu->S[inst.destination] = cpu->S[inst.source_1] & cpu->S[inst.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void ors(CPU *cpu, Instruction inst)
{
    cpu->S[inst.destination] = cpu->S[inst.source_1] | cpu->S[inst.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void xors(CPU *cpu, Instruction inst)
{
    cpu->S[inst.destination] = cpu->S[inst.source_1] ^ cpu->S[inst.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void shls(CPU *cpu, Instruction inst)
{
    // TODO : gestion de l'overflow

    cpu->S[inst.destination] = cpu->S[inst.source_1] << cpu->S[inst.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void shrs(CPU *cpu, Instruction inst)
{
    // TODO : gestion de l'overflow

    cpu->S[inst.destination] = cpu->S[inst.source_1] >> cpu->S[inst.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void rols(CPU *cpu, Instruction inst)
{
    cpu->S[inst.destination] = cpu->S[inst.source_1] << ~cpu->S[inst.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void rors(CPU *cpu, Instruction inst)
{
    cpu->S[inst.destination] = cpu->S[inst.source_1] >> ~cpu->S[inst.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void popcnts(CPU *cpu, Instruction inst)
{
    cpu->U[inst.destination] = __builtin_popcountll(cpu->U[inst.source_1]);
    cpu->IP += SIZE_INSTRUCTION;
}

void lmbs(CPU *cpu, Instruction inst)
{
    // TODO
    cpu->IP += SIZE_INSTRUCTION;
}
