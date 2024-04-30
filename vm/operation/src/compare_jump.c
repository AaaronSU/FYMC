#include "cpu.h"
#include "type.h"
#include "tools.h"

void cmpu(CPU *cpu, Instruction instruction)
{
    u64 u0 = cpu->U[instruction.destination];
    u64 u1 = cpu->U[instruction.source_1];

    cpu->CF |= (u0 == u1) << 7;
    cpu->CF |= (u0 != u1) << 6;
    cpu->CF |= (u0 > u1) << 5;
    cpu->CF |= (u0 >= u1) << 4;
    cpu->CF |= (u0 < u1) << 3;
    cpu->CF |= (u0 <= u1) << 2;
}
void cmps(CPU *cpu, Instruction instruction)
{
    i64 s0 = cpu->S[instruction.destination];
    i64 s1 = cpu->S[instruction.source_1];

    cpu->CF |= (s0 == s1) << 7;
    cpu->CF |= (s0 != s1) << 6;
    cpu->CF |= (s0 > s1) << 5;
    cpu->CF |= (s0 >= s1) << 4;
    cpu->CF |= (s0 < s1) << 3;
    cpu->CF |= (s0 <= s1) << 2;
}

void cmpf(CPU *cpu, Instruction instruction)
{
    f64 f0 = cpu->F[instruction.destination];
    f64 f1 = cpu->F[instruction.source_1];

    cpu->CF |= (f0 == f1) << 7;
    cpu->CF |= (f0 != f1) << 6;
    cpu->CF |= (f0 > f1) << 5;
    cpu->CF |= (f0 >= f1) << 4;
    cpu->CF |= (f0 < f1) << 3;
    cpu->CF |= (f0 <= f1) << 2;
}
void je(CPU *cpu, Instruction instruction)
{
    (void)instruction; // Suppress unused parameter warning
    cpu->IP = (cpu->CF & (1 << 7)) ? (u64)(get_immediate(cpu) / 8) : cpu->IP + 1;
}

void jne(CPU *cpu, Instruction instruction)
{
    (void)instruction; // Suppress unused parameter warning
    cpu->IP = (cpu->CF & (1 << 6)) ? (u64)(get_immediate(cpu) / 8) : cpu->IP + 1;
}

void jg(CPU *cpu, Instruction instruction)
{
    (void)instruction; // Suppress unused parameter warning
    cpu->IP = (cpu->CF & (1 << 5)) ? (u64)(get_immediate(cpu) / 8) : cpu->IP + 1;
}

void jge(CPU *cpu, Instruction instruction)
{
    (void)instruction; // Suppress unused parameter warning
    cpu->IP = (cpu->CF & (1 << 4)) ? (u64)(get_immediate(cpu) / 8) : cpu->IP + 1;
}

void jl(CPU *cpu, Instruction instruction)
{
    (void)instruction; // Suppress unused parameter warning
    cpu->IP = (cpu->CF & (1 << 3)) ? (u64)(get_immediate(cpu) / 8) : cpu->IP + 1;
}

void jle(CPU *cpu, Instruction instruction)
{
    (void)instruction; // Suppress unused parameter warning
    cpu->IP = (cpu->CF & (1 << 2)) ? (u64)(get_immediate(cpu) / 8) : cpu->IP + 1;
}

void jz(CPU *cpu, Instruction instruction)
{
    (void)instruction; // Suppress unused parameter warning
    cpu->IP = (cpu->CF & 1) ? (u64)(get_immediate(cpu) / 8) : cpu->IP + 1;
}

void jnz(CPU *cpu, Instruction instruction)
{
    (void)instruction; // Suppress unused parameter warning
    cpu->IP = (!(cpu->CF & 1)) ? (u64)(get_immediate(cpu) / 8) : cpu->IP + 1;
}
