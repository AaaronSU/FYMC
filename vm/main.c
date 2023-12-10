#include "instruction.h"
#include "init.h"
#include "type.h"
#include <stdio.h>

struct Instruction
{
    u8 opcode : 8;
    u16 unused : 9;
    u8 destination : 5;
    u8 source_1 : 5;
    u8 source_2 : 5;
};

// Tableau de pointeurs de fonction vers les fonctions d'opcodes
// dépanage => utilisation de la fonction tools pour extraire les données
void (*opcode_functions[])(u8, u8, CPU*) = {movu, movs, movf, movv, movt, movg, movui, movsi, movfi, movvi, movti, movgi};

// Fonction pour exécuter l'instruction
void run_instruction(void (*opcode_function)(u8, u8, CPU*), struct Instruction instruction, CPU* cpu) 
{
    opcode_function(instruction.destination, instruction.source_1, cpu);
}

int main() {
    struct Instruction instruction;
    CPU cpu;
    // Initialisation de valeurs pour le test
    cpu.U[1] = 5;
    cpu.U[3] = 1;
    cpu.S[15] = 54478;
    cpu.S[0] = 1; 

    // Initialisation de test pour l'instruction
    u32 test[] = {0b00000000000000000000010001100000, 0b00000001000000000000000111100000};

    // Décomposition de l'instruction
    for(int i = 0; i < 2; i++) 
    {
        instruction.opcode = (test[i] >> (SIZE_INSTRUCTION - 8)) & 0xFF;
        instruction.unused = (test[i] >> (SIZE_INSTRUCTION - 17)) & 0x1FF; 
        instruction.destination = (test[i] >> (SIZE_INSTRUCTION - 22)) & 0x1F; 
        instruction.source_1 = (test[i] >> (SIZE_INSTRUCTION - 27)) & 0x1F; 
        instruction.source_2 = test[i] & 0x1F;

        // Exécution de l'instruction basée sur l'opcode
        run_instruction(opcode_functions[instruction.opcode], instruction, &cpu);
    }

    // Affichage des résultats
    printf("U[1]: %llu\n", cpu.U[1]);
    printf("U[3]: %llu\n", cpu.U[3]);
    printf("S[15]: %llu\n", cpu.S[15]);
    printf("S[0]: %llu\n", cpu.S[0]);

    printf("opcode: %u\n", instruction.opcode);
    printf("unused: %u\n", instruction.unused);
    printf("destination: %u\n", instruction.destination);
    printf("source_1: %u\n", instruction.source_1);
    printf("source_2: %u\n", instruction.source_2);
    return 0;
}
    

