#include "../vm.h"
#include "../instruction.h"

void fmas(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant FMAF--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n",
                instruction.register_1, core->S[instruction.register_1],
                instruction.register_2, core->S[instruction.register_2],
                instruction.register_3, core->S[instruction.register_3]);
    core->S[instruction.register_1] += core->S[instruction.register_2] * core->S[instruction.register_3];
    DEBUG_PRINT("--------Avant FMAF--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n",
                instruction.register_1, core->S[instruction.register_1],
                instruction.register_2, core->S[instruction.register_2],
                instruction.register_3, core->S[instruction.register_3]);
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}