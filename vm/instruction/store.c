#include "../vm.h"
#include "../instruction.h"

void storeu(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant STOREU--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "offset a pour valeur %d\n"
                "core->memory[**address**] a pour valeur %ld\n",
                instruction.register_1, core->U[instruction.register_1],
                instruction.register_2, core->U[instruction.register_2],
                instruction.register_3, core->U[instruction.register_3],
                instruction.offset,
                *(u64 *)&(core->memory[core->U[instruction.register_1] + core->U[instruction.register_2] + instruction.offset]));

    memcpy(&core->memory[core->U[instruction.register_1] + core->U[instruction.register_2] + instruction.offset], &core->U[instruction.register_3], 64);

    DEBUG_PRINT("--------Après STOREU--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "offset a pour valeur %d\n"
                "core->memory[**address**] a pour valeur %ld\n\n",
                instruction.register_1, core->U[instruction.register_1],
                instruction.register_2, core->U[instruction.register_2],
                instruction.register_3, core->U[instruction.register_3],
                instruction.offset,
                *(u64 *)&(core->memory[core->U[instruction.register_1] + core->U[instruction.register_2] + instruction.offset]));
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

void stores(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    memcpy(&core->memory[core->U[instruction.register_1] + core->U[instruction.register_2] + instruction.offset], &core->S[instruction.register_3], 64);
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

void storef(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant STOREU--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "offset a pour valeur %d\n"
                "core->memory[**address**] a pour valeur %f\n",
                instruction.register_1, core->U[instruction.register_1],
                instruction.register_2, core->U[instruction.register_2],
                instruction.register_3, core->U[instruction.register_3],
                instruction.offset,
                core->F[instruction.register_3]);

    memcpy(&core->memory[core->U[instruction.register_1] + core->U[instruction.register_2] + instruction.offset], &core->F[instruction.register_3], 64);

    DEBUG_PRINT("--------Après STOREU--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "offset a pour valeur %d\n"
                "core->memory[**address**] a pour valeur %f\n\n",
                instruction.register_1, core->U[instruction.register_1],
                instruction.register_2, core->U[instruction.register_2],
                instruction.register_3, core->U[instruction.register_3],
                instruction.offset,
                core->F[instruction.register_3]);
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}