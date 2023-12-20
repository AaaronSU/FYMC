#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>

#include "u_arithmetic_logic.h"
#include "tools.h"

void addu(CPU *cpu, Instruction instruction) 
{
    // TODO : overflow à gérer
    
    cpu->U[instruction.destination] = cpu->U[instruction.source_1] + cpu->U[instruction.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void subu(CPU *cpu, Instruction instruction) 
{
    
    if( cpu->U[instruction.source_1] < cpu->U[instruction.source_2]) 
    {
        cpu->CF += 1; // 0: an unsigned subtraction with the first source operand lower than the second source operand
        exit(EXIT_FAILURE);
    }
    cpu->U[instruction.destination] = cpu->U[instruction.source_1] - cpu->U[instruction.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void mulu(CPU *cpu, Instruction instruction) {
    
    cpu->U[instruction.destination] = cpu->U[instruction.source_1] * cpu->U[instruction.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}


void divu(CPU *cpu, Instruction instruction) {
    
    // the second source operand of a division or a modulo instruction is null
    if (cpu->U[instruction.source_2] == 0) 
    {
        cpu->CF += 1;
        exit(EXIT_FAILURE); 
    }
    
    cpu->U[instruction.destination] = cpu->U[instruction.source_1] / cpu->U[instruction.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void modu(CPU *cpu, Instruction instruction) {
    
    // the second source operand of a division or a modulo instruction is null
    if (cpu->U[instruction.source_2] == 0) 
    {
        cpu->CF += 1;
        exit(EXIT_FAILURE);
    }
    cpu->U[instruction.destination] = cpu->U[instruction.source_1] % cpu->U[instruction.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void fmau(CPU *cpu, Instruction instruction) {
    // TODO : overflow à gerer
    
    cpu->U[instruction.destination] += cpu->U[instruction.source_1] * cpu->U[instruction.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void sqrtu(CPU *cpu, Instruction instruction) {
    
    cpu->U[instruction.destination] = sqrt(cpu->U[instruction.source_1]);
    cpu->IP += SIZE_INSTRUCTION;
}

void logu(CPU *cpu, Instruction instruction) {
    
    if (cpu->U[instruction.source_1] == 0) 
    {
        exit(EXIT_FAILURE); // mieux gerer :(
    }
    cpu->U[instruction.destination] = log(cpu->U[instruction.source_1]);
    cpu->IP += SIZE_INSTRUCTION;
}

void incu(CPU *cpu, Instruction instruction) {
    
    if (cpu->U[instruction.destination] < LLONG_MAX) {
        cpu->U[instruction.destination]++;
    } else {
        exit(EXIT_FAILURE); // mieux gerer :(
    }
    cpu->IP += SIZE_INSTRUCTION;
}

void decu(CPU *cpu, Instruction instruction) {
    
    if (cpu->U[instruction.destination] > 0) {
        cpu->U[instruction.destination]--;
    } else {
        exit(EXIT_FAILURE); // mieux gerer :(
    }
    cpu->IP += SIZE_INSTRUCTION;
}

void andu(CPU *cpu, Instruction instruction) {
    
    cpu->U[instruction.destination] = cpu->U[instruction.source_1] & cpu->U[instruction.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void oru(CPU *cpu, Instruction instruction) {
    
    cpu->U[instruction.destination] = cpu->U[instruction.source_1] | cpu->U[instruction.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void xoru(CPU *cpu, Instruction instruction) {
    
    cpu->U[instruction.destination] = cpu->U[instruction.source_1] ^ cpu->U[instruction.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void shlu(CPU *cpu, Instruction instruction) {
    // TODO : gerer l'overflow du shift
    
    cpu->U[instruction.destination] = cpu->U[instruction.source_1] << cpu->U[instruction.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void shru(CPU *cpu, Instruction instruction) {
    // TODO : gerer l'overflow du shift
    
    cpu->U[instruction.destination] = cpu->U[instruction.source_1] >> cpu->U[instruction.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void rolu(CPU *cpu, Instruction instruction) {
    
    cpu->S[instruction.destination] = cpu->S[instruction.source_1] << ~cpu->S[instruction.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void roru(CPU *cpu, Instruction instruction) {
    
    cpu->U[instruction.destination] = cpu->U[instruction.source_1] >> ~cpu->U[instruction.source_2];
    cpu->IP += SIZE_INSTRUCTION;
}

void popcntu(CPU *cpu, Instruction instruction) {
    // Hamming weight : utilisation de c mais peut-etre à la main ?
    
    cpu->U[instruction.destination] = __builtin_popcountll(cpu->U[instruction.source_1]);
    cpu->IP += SIZE_INSTRUCTION;
}

void lmbu(CPU *cpu, Instruction instruction) {
    //TODO
}
