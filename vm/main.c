#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "opcode.h"

#include "type.h"
#include "tools.h"
#include "cpu.h"
#include "load.h"
#include "conversion.h"
#include "store.h"
#include "move.h"


void (*opcode_functions[])(CPU*, Instruction) = {
    loadu, loads, loadf,        // 0, 1, 2
    loadv, loadt, loadg,        // 3, 4, 5
    storeu, stores, storef,     // 6, 7, 8
    storev, storet, storeg,     // 9, 10, 11
    movu, movs, movf, movv, movt, movg,   // 12, 13, 14, 15, 16, 17, 18
    //movui, movsi, movfi, movvi, movti, movgi,   // 19, 20, 21, 22, 23, 24
    cvtus, cvtsu, cvtuf, cvtfu, cvtsf, cvtfs,  // 25, 26, 27, 28, 29, 30
    //addu, subu, mulu, divu, modu, fmau, sqrtu, logu, incu, decu, andu, oru, xoru, shlu, shru, rolu, roru, popcntu, lmbu, // 31-50
    //adds, subs, muls, divs, mods, fmas, sqrts, logs, ands, ors, xors, shls, shrs, rols, rors, popcnts, lmbs,  // 51-69
    //addf, subf, mulf, divf, fmaf, sqrtf, logf,   // 70-76
    //cmpu, cmps, cmpf, je, jne, jge, jl, jle, jz, jnz,  // 77-86
    //outu, outs, outf, outa, outb, outx,   // 87-92
    //hlt   // 93
};

void run_instruction(CPU *cpu, Instruction instruction, u32 instr, void (*opcode_functions[])(CPU*, Instruction)) {

    // 1 - décoder les 32 bits et voir quel opérand
    instruction = parse_instruction(instr); // 
    (*opcode_functions[instruction.opcode])(cpu, instruction);
}


int main()
{
    CPU cpu;
    
    memset(&cpu, 0, sizeof(CPU)); // Initialize cpu to zero

    // Initialisation de test pour l'instruction


    //
    u32 test[] = {0b00000000000000000000010001100000, 0b00000001000000000000000111100000};
    Instruction instruction = parse_instruction(test[0]);

    for(int i = 7; i < 15; i++) 
    {
        cpu.Memory[i] = i * 2;
    }

    cpu.U[3] = 2;
    cpu.U[0] = 5;
    loadv(&cpu, instruction);
    
    printf("opcode: %u\n", instruction.opcode);
    printf("unused: %u\n", instruction.unused);
    printf("destination: %u\n", instruction.destination);
    printf("source_1: %u\n", instruction.source_1);
    printf("source_2: %u\n", instruction.source_2);

    cpu.S[1] = -1;
    cpu.U[3] = 1ULL << 62;
    cvtus(&cpu, instruction);

    

    printf("U[3] : %llu, S[1] : %lld\n", cpu.U[3], cpu.S[1]);
    /* printf("Memory à 7 : %llu, U[3] : %llu, U[0] : %llu\n", cpu.Memory[7], cpu.U[3], cpu.U[0]);
    for(int i = 0; i < 8; i++) 
    {
         printf("Destination : %llu\n", cpu.V[1][i]);
    }
    */

    /* // Décomposition de l'instruction
    for (int i = 0; i < 2; i++)
    {
        instruction.opcode = (test[i] >> (SIZE_INSTRUCTION - 8)) & 0xFF;
        instruction.unused = (test[i] >> (SIZE_INSTRUCTION - 17)) & 0x1FF;
        instruction.destination = (test[i] >> (SIZE_INSTRUCTION - 22)) & 0x1F;
        instruction.source_1 = (test[i] >> (SIZE_INSTRUCTION - 27)) & 0x1F;
        instruction.source_2 = test[i] & 0x1F;

        execute_instruction(
            &cpu,
            instruction.opcode,
            instruction.unused,
            instruction.destination,
            instruction.source_1,
            instruction.source_2);

        // Affichage des résultats
        printf("U[1]: %llu\n", cpu.scalar_regs.U[1]);
        printf("U[2]: %llu\n", cpu.scalar_regs.U[2]);
        printf("U[3]: %llu\n", cpu.scalar_regs.U[3]);
        printf("S[15]: %llu\n", cpu.scalar_regs.S[15]);
        printf("S[0]: %llu\n", cpu.scalar_regs.S[0]);

        
    }

    execute_instruction(&cpu, ADDU, 0, 1, 2, 3); */

    return 0;
}