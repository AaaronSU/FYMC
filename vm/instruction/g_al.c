#include "../vm.h"
#include "../instruction.h"

void sumg(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    for (u8 indice = 1; indice < NUMBER_SCALAR_IN_VECTOR_REGISTER; indice++)
    {
        core->G[instruction.register_1][0] += core->G[instruction.register_1][indice];
        // printf("%f\n", core->G[instruction.register_1][indice]);
    }
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

void fmag(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));

    for (u8 indice = 0; indice < NUMBER_SCALAR_IN_VECTOR_REGISTER; indice++)
    {
        DEBUG_PRINT("--------Avant FMAG[%d] --------\n", indice);
        DEBUG_PRINT("Le registre %d[%d] a pour valeur %f\n"
                    "Le registre %d[%d] a pour valeur %f\n"
                    "Le registre %d[%d] a pour valeur %f\n",
                    instruction.register_1, indice, core->G[instruction.register_1][indice],
                    instruction.register_2, indice, core->G[instruction.register_2][indice],
                    instruction.register_3, indice, core->G[instruction.register_3][indice]);

        core->G[instruction.register_1][indice] += core->G[instruction.register_2][indice] * core->G[instruction.register_3][indice];

        DEBUG_PRINT("--------Avant FMAG[%d] --------\n", indice);
        DEBUG_PRINT("Le registre %d[%d] a pour valeur %f\n"
                    "Le registre %d[%d] a pour valeur %f\n"
                    "Le registre %d[%d] a pour valeur %f\n\n",
                    instruction.register_1, indice, core->G[instruction.register_1][indice],
                    instruction.register_2, indice, core->G[instruction.register_2][indice],
                    instruction.register_3, indice, core->G[instruction.register_3][indice]);
        // printf("%f\n", core->G[instruction.register_1][indice]);
    }
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}