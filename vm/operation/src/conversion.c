#include "conversion.h"
#include "tools.c"


void cvtus(CPU *cpu, u32 instruction) 
{
    // S0, U0
    Instruction inst = parse_instruction(instruction);
    cpu->S[inst.destination] = (i64)cpu->U[inst.source_1];
    cpu->IP += SIZE_INSTRUCTION;
}

void cvtsu(CPU *cpu, u32 instruction)
{
    Instruction inst = parse_instruction(instruction);
    cpu->U[inst.destination] = (u64)cpu->S[inst.source_1];
    cpu->IP += SIZE_INSTRUCTION;
}

void cvtuf(CPU *cpu, u32 instruction)
{
    Instruction inst = parse_instruction(instruction);
    cpu->F[inst.destination] = (f64)cpu->U[inst.source_1];
    cpu->IP += SIZE_INSTRUCTION;
}
void cvtfu(CPU *cpu, u32 instruction)
{
    Instruction inst = parse_instruction(instruction);
    cpu->U[inst.destination] = (u64)cpu->F[inst.source_1];
    cpu->IP += SIZE_INSTRUCTION;
}
void cvtsf(CPU *cpu, u32 instruction)
{
    Instruction inst = parse_instruction(instruction);
    cpu->F[inst.destination] = (f64)cpu->S[inst.source_1];
    cpu->IP += SIZE_INSTRUCTION;
}
void cvtfs(CPU *cpu, u32 instruction)
{
    Instruction inst = parse_instruction(instruction);
    cpu->S[inst.destination] = (i64)cpu->F[inst.source_1];
    cpu->IP += SIZE_INSTRUCTION;
}