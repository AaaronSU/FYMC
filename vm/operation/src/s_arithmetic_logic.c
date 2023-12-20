#include <stdlib.h>
#include <math.h>
#include "s_arithmetic_logic.h"
#include "cpu.h"
#include "tools.h"

void adds(CPU *cpu, Instruction instruction) {
    // TODO : gestion de l'overflow
    
    cpu->S[instruction.destination] = cpu->S[instruction.source_1] + cpu->S[instruction.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void subs(CPU *cpu, Instruction instruction) {
    // TODO : gestion de l'overflow
    
    cpu->S[instruction.destination] = cpu->S[instruction.source_1] - cpu->S[instruction.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void muls(CPU *cpu, Instruction instruction) {
    
    // TODO : gestion de l'overflow
    cpu->S[instruction.destination] = cpu->S[instruction.source_1] * cpu->S[instruction.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void divs(CPU *cpu, Instruction instruction) {
    
    if (cpu->S[instruction.source_2] == 0) {
        cpu->CF += 1; // Erreur : division par zéro
        exit(EXIT_FAILURE);
    }
    cpu->S[instruction.destination] = cpu->S[instruction.source_1] / cpu->S[instruction.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void mods(CPU *cpu, Instruction instruction) {
    
    if (cpu->S[instruction.source_2] == 0) {
        cpu->CF += 1; // Erreur : modulo par zéro
        exit(EXIT_FAILURE);
    }
    cpu->S[instruction.destination] = cpu->S[instruction.source_1] % cpu->S[instruction.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void fmas(CPU *cpu, Instruction instruction) {
    // TODO : gestion de l'overflow
    
    cpu->S[instruction.destination] += cpu->S[instruction.source_1] * cpu->S[instruction.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void sqrts(CPU *cpu, Instruction instruction) {
    
    if (cpu->S[instruction.source_1] < 0) 
    {
        exit(EXIT_FAILURE);
    }
    cpu->S[instruction.destination] = sqrt(cpu->S[instruction.source_1]);
    cpu->IP += SIZE_INSTRUCTION;
}

void logs(CPU *cpu, Instruction instruction) {
    
    if (cpu->S[instruction.source_1] <= 0) 
    {
        exit(EXIT_FAILURE);
    }
    cpu->S[instruction.destination] = log(cpu->S[instruction.source_1]);
    cpu->IP += SIZE_INSTRUCTION;
}

void ands(CPU *cpu, Instruction instruction) {
    
    cpu->S[instruction.destination] = cpu->S[instruction.source_1] & cpu->S[instruction.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void ors(CPU *cpu, Instruction instruction) {
    
    cpu->S[instruction.destination] = cpu->S[instruction.source_1] | cpu->S[instruction.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void xors(CPU *cpu, Instruction instruction) {
    
    cpu->S[instruction.destination] = cpu->S[instruction.source_1] ^ cpu->S[instruction.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void shls(CPU *cpu, Instruction instruction) {
    // TODO : gestion de l'overflow
    
    cpu->S[instruction.destination] = cpu->S[instruction.source_1] << cpu->S[instruction.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void shrs(CPU *cpu, Instruction instruction) {
    // TODO : gestion de l'overflow
    
    cpu->S[instruction.destination] = cpu->S[instruction.source_1] >> cpu->S[instruction.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void rols(CPU *cpu, Instruction instruction) {
    
    cpu->S[instruction.destination] = cpu->S[instruction.source_1] << ~cpu->S[instruction.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void rors(CPU *cpu, Instruction instruction) {
    
    cpu->S[instruction.destination] = cpu->S[instruction.source_1] >> ~cpu->S[instruction.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void popcnts(CPU *cpu, Instruction instruction) {
    
    cpu->U[instruction.destination] = __builtin_popcountll(cpu->U[instruction.source_1]);
    cpu->IP += SIZE_INSTRUCTION;
}

void lmbs(CPU *cpu, Instruction instruction) {
    
    //TODO
    cpu->IP += SIZE_INSTRUCTION;
}
