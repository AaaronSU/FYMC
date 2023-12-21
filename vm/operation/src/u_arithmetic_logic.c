#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>

#include "u_arithmetic_logic.h"
#include "tools.h"

void addu(CPU *cpu, u32 instruction)
{
    // TODO : overflow à gérer
    Instruction inst = parse_instruction(instruction);
    cpu->U[inst.destination] = cpu->U[inst.source_1] + cpu->U[inst.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void subu(CPU *cpu, u32 instruction)
{
    Instruction inst = parse_instruction(instruction);
    if (cpu->U[inst.source_1] < cpu->U[inst.source_2])
    {
        cpu->CF += 1; // 0: an unsigned subtraction with the first source operand lower than the second source operand
        exit(EXIT_FAILURE);
    }
    cpu->U[inst.destination] = cpu->U[inst.source_1] - cpu->U[inst.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void mulu(CPU *cpu, u32 instruction)
{
    Instruction inst = parse_instruction(instruction);
    cpu->U[inst.destination] = cpu->U[inst.source_1] * cpu->U[inst.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void divu(CPU *cpu, u32 instruction)
{
    Instruction inst = parse_instruction(instruction);
    // the second source operand of a division or a modulo instruction is null
    if (cpu->U[inst.source_2] == 0)
    {
        cpu->CF += 1;
        exit(EXIT_FAILURE);
    }

    cpu->U[inst.destination] = cpu->U[inst.source_1] / cpu->U[inst.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void modu(CPU *cpu, u32 instruction)
{
    Instruction inst = parse_instruction(instruction);
    // the second source operand of a division or a modulo instruction is null
    if (cpu->U[inst.source_2] == 0)
    {
        cpu->CF += 1;
        exit(EXIT_FAILURE);
    }
    cpu->U[inst.destination] = cpu->U[inst.source_1] % cpu->U[inst.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void fmau(CPU *cpu, u32 instruction)
{
    // TODO : overflow à gerer
    Instruction inst = parse_instruction(instruction);
    cpu->U[inst.destination] += cpu->U[inst.source_1] * cpu->U[inst.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void sqrtu(CPU *cpu, u32 instruction)
{
    Instruction inst = parse_instruction(instruction);
    cpu->U[inst.destination] = sqrt(cpu->U[inst.source_1]);
    cpu->IP += SIZE_INSTRUCTION;
}

void logu(CPU *cpu, u32 instruction)
{
    Instruction inst = parse_instruction(instruction);
    if (cpu->U[inst.source_1] == 0)
    {
        exit(EXIT_FAILURE); // mieux gerer :(
    }
    cpu->U[inst.destination] = log(cpu->U[inst.source_1]);
    cpu->IP += SIZE_INSTRUCTION;
}

void incu(CPU *cpu, u32 instruction)
{
    Instruction inst = parse_instruction(instruction);
    if (cpu->U[inst.destination] < LLONG_MAX)
    {
        cpu->U[inst.destination]++;
    }
    else
    {
        exit(EXIT_FAILURE); // mieux gerer :(
    }
    cpu->IP += SIZE_INSTRUCTION;
}

void decu(CPU *cpu, u32 instruction)
{
    Instruction inst = parse_instruction(instruction);
    if (cpu->U[inst.destination] > 0)
    {
        cpu->U[inst.destination]--;
    }
    else
    {
        exit(EXIT_FAILURE); // mieux gerer :(
    }
    cpu->IP += SIZE_INSTRUCTION;
}

void andu(CPU *cpu, u32 instruction)
{
    Instruction inst = parse_instruction(instruction);
    cpu->U[inst.destination] = cpu->U[inst.source_1] & cpu->U[inst.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void oru(CPU *cpu, u32 instruction)
{
    Instruction inst = parse_instruction(instruction);
    cpu->U[inst.destination] = cpu->U[inst.source_1] | cpu->U[inst.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void xoru(CPU *cpu, u32 instruction)
{
    Instruction inst = parse_instruction(instruction);
    cpu->U[inst.destination] = cpu->U[inst.source_1] ^ cpu->U[inst.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void shlu(CPU *cpu, u32 instruction)
{
    // TODO : gerer l'overflow du shift
    Instruction inst = parse_instruction(instruction);
    cpu->U[inst.destination] = cpu->U[inst.source_1] << cpu->U[inst.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void shru(CPU *cpu, u32 instruction)
{
    // TODO : gerer l'overflow du shift
    Instruction inst = parse_instruction(instruction);
    cpu->U[inst.destination] = cpu->U[inst.source_1] >> cpu->U[inst.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void rolu(CPU *cpu, u32 instruction)
{
    Instruction inst = parse_instruction(instruction);
    cpu->S[inst.destination] = cpu->S[inst.source_1] << ~cpu->S[inst.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void roru(CPU *cpu, u32 instruction)
{
    Instruction inst = parse_instruction(instruction);
    cpu->U[inst.destination] = cpu->U[inst.source_1] >> ~cpu->U[inst.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void popcntu(CPU *cpu, u32 instruction)
{
    // Hamming weight : utilisation de c mais peut-etre à la main ?
    Instruction inst = parse_instruction(instruction);
    cpu->U[inst.destination] = __builtin_popcountll(cpu->U[inst.source_1]);
    cpu->IP += SIZE_INSTRUCTION;
}

void lmbu(CPU *cpu, u32 instruction)
{
    // TODO
}
