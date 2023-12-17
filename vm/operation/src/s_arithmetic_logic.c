#include <stdlib.h>
#include <math.h>
#include "s_arithmetic_logic.h"
#include "cpu.h"
#include "tools.h"

void adds(CPU *cpu, u32 instruction) {
    // TODO : gestion de l'overflow
    Instruction inst = parse_instruction(instruction);
    cpu->S[inst.destination] = cpu->S[inst.source_1] + cpu->S[inst.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void subs(CPU *cpu, u32 instruction) {
    // TODO : gestion de l'overflow
    Instruction inst = parse_instruction(instruction);
    cpu->S[inst.destination] = cpu->S[inst.source_1] - cpu->S[inst.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void muls(CPU *cpu, u32 instruction) {
    Instruction inst = parse_instruction(instruction);
    // TODO : gestion de l'overflow
    cpu->S[inst.destination] = cpu->S[inst.source_1] * cpu->S[inst.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void divs(CPU *cpu, u32 instruction) {
    Instruction inst = parse_instruction(instruction);
    if (cpu->S[inst.source_2] == 0) {
        cpu->CF += 1; // Erreur : division par zéro
        exit(EXIT_FAILURE);
    }
    cpu->S[inst.destination] = cpu->S[inst.source_1] / cpu->S[inst.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void mods(CPU *cpu, u32 instruction) {
    Instruction inst = parse_instruction(instruction);
    if (cpu->S[inst.source_2] == 0) {
        cpu->CF += 1; // Erreur : modulo par zéro
        exit(EXIT_FAILURE);
    }
    cpu->S[inst.destination] = cpu->S[inst.source_1] % cpu->S[inst.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void fmas(CPU *cpu, u32 instruction) {
    // TODO : gestion de l'overflow
    Instruction inst = parse_instruction(instruction);
    cpu->S[inst.destination] += cpu->S[inst.source_1] * cpu->S[inst.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void sqrts(CPU *cpu, u32 instruction) {
    Instruction inst = parse_instruction(instruction);
    if (cpu->S[inst.source_1] < 0) 
    {
        exit(EXIT_FAILURE);
    }
    cpu->S[inst.destination] = sqrt(cpu->S[inst.source_1]);
    cpu->IP += SIZE_INSTRUCTION;
}

void logs(CPU *cpu, u32 instruction) {
    Instruction inst = parse_instruction(instruction);
    if (cpu->S[inst.source_1] <= 0) 
    {
        exit(EXIT_FAILURE);
    }
    cpu->S[inst.destination] = log(cpu->S[inst.source_1]);
    cpu->IP += SIZE_INSTRUCTION;
}

void ands(CPU *cpu, u32 instruction) {
    Instruction inst = parse_instruction(instruction);
    cpu->S[inst.destination] = cpu->S[inst.source_1] & cpu->S[inst.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void ors(CPU *cpu, u32 instruction) {
    Instruction inst = parse_instruction(instruction);
    cpu->S[inst.destination] = cpu->S[inst.source_1] | cpu->S[inst.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void xors(CPU *cpu, u32 instruction) {
    Instruction inst = parse_instruction(instruction);
    cpu->S[inst.destination] = cpu->S[inst.source_1] ^ cpu->S[inst.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void shls(CPU *cpu, u32 instruction) {
    // TODO : gestion de l'overflow
    Instruction inst = parse_instruction(instruction);
    cpu->S[inst.destination] = cpu->S[inst.source_1] << cpu->S[inst.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void shrs(CPU *cpu, u32 instruction) {
    // TODO : gestion de l'overflow
    Instruction inst = parse_instruction(instruction);
    cpu->S[inst.destination] = cpu->S[inst.source_1] >> cpu->S[inst.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void rols(CPU *cpu, u32 instruction) {
    Instruction inst = parse_instruction(instruction);
    cpu->S[inst.destination] = cpu->S[inst.source_1] << ~cpu->S[inst.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void rors(CPU *cpu, u32 instruction) {
    Instruction inst = parse_instruction(instruction);
    cpu->S[inst.destination] = cpu->S[inst.source_1] >> ~cpu->S[inst.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void popcnts(CPU *cpu, u32 instruction) {
    Instruction inst = parse_instruction(instruction);
    cpu->U[inst.destination] = __builtin_popcountll(cpu->U[inst.source_1]);
    cpu->IP += SIZE_INSTRUCTION;
}

void lmbs(CPU *cpu, u32 instruction) {
    Instruction inst = parse_instruction(instruction);
    //TODO
    cpu->IP += SIZE_INSTRUCTION;
}
