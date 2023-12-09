#include "instruction.h"
#include "init.h"

#include <stdio.h>
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
    u32 test = 0b00000000000000000000010000100000;
    struct Instruction instruction;

    instruction.opcode = (test >> (SIZE_INSTRUCTION - 8)) & 0xFF;
    instruction.unused = (test >> (SIZE_INSTRUCTION - 17)) & 0x1FF; 
    instruction.destination = (test >> (SIZE_INSTRUCTION - 22)) & 0x1F; 
    instruction.source_1 = (test >> (SIZE_INSTRUCTION - 27)) & 0x1F; 
    instruction.source_2 = test & 0x1F;
    
    printf("opcode: %u\n", instruction.opcode);
    printf("unused: %u\n", instruction.unused);
    printf("destination: %u\n", instruction.destination);
    printf("source_1: %u\n", instruction.source_1);
    printf("source_2: %u\n", instruction.source_2);

    return 0;
}














// chargement du fichier binaire
// vérification de la cohérence du header et du fichier binaire
// décodage des intructions -> découpage de l'opcode et des opérands 
// exécution des instructions -> exécution des opérations et gestions de la mémoire