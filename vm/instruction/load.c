#include "../vm.h"
#include "../instruction.h"

void loadu(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant LOADU--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "core->memory[**address**] a pour valeur %lx\n",
                instruction.register_1, core->U[instruction.register_1],
                instruction.register_2, core->U[instruction.register_2],
                instruction.register_3, core->U[instruction.register_3],
                *(u64 *)&(core->memory[core->U[instruction.register_2] + core->U[instruction.register_3] + instruction.offset]));
    core->U[instruction.register_1] = *(u64 *)&(core->memory[core->U[instruction.register_2] + core->U[instruction.register_3] + instruction.offset]);
    DEBUG_PRINT("--------Après LOADU--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "core->memory[**address**] a pour valeur %lx\n\n",
                instruction.register_1, core->U[instruction.register_1],
                instruction.register_2, core->U[instruction.register_2],
                instruction.register_3, core->U[instruction.register_3],
                *(u64 *)&(core->memory[core->U[instruction.register_2] + core->U[instruction.register_3] + instruction.offset]));
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}