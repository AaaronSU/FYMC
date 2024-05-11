#include "../vm.h"
#include "../instruction.h"

void fmaf_(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant FMAF--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %f\n"
                "Le registre %d a pour valeur %f\n"
                "Le registre %d a pour valeur %f\n",
                instruction.register_1, core->F[instruction.register_1],
                instruction.register_2, core->F[instruction.register_2],
                instruction.register_3, core->F[instruction.register_3]);
    core->F[instruction.register_1] += core->F[instruction.register_2] * core->F[instruction.register_3];
    DEBUG_PRINT("--------Avant FMAF--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %f\n"
                "Le registre %d a pour valeur %f\n"
                "Le registre %d a pour valeur %f\n",
                instruction.register_1, core->F[instruction.register_1],
                instruction.register_2, core->F[instruction.register_2],
                instruction.register_3, core->F[instruction.register_3]);
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}