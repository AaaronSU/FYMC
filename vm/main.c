#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "registre.h"
#include "opcode.h"
#include "exec_ins.h"

struct Instruction
{
    u8 opcode : 8;
    u16 unused : 9;
    u8 destination : 5;
    u8 source_1 : 5;
    u8 source_2 : 5;
};

int main()
{
    CPU cpu;
    struct Instruction instruction;
    memset(&cpu, 0, sizeof(CPU)); // Initialize cpu to zero

    cpu.scalar_regs.U[2] = 5;
    cpu.scalar_regs.U[3] = 1;
    cpu.scalar_regs.S[15] = 54478;
    cpu.scalar_regs.S[0] = 1;
    // Initialisation de test pour l'instruction

    //
    u32 test[] = {0b00000000000000000000010001100000, 0b00000001000000000000000111100000};

    // Décomposition de l'instruction
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

        printf("opcode: %u\n", instruction.opcode);
        printf("unused: %u\n", instruction.unused);
        printf("destination: %u\n", instruction.destination);
        printf("source_1: %u\n", instruction.source_1);
        printf("source_2: %u\n", instruction.source_2);
    }

    execute_instruction(&cpu, ADDU, 0, 1, 2, 3);

    return 0;
}