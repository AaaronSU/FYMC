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

void loads(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    core->S[instruction.register_1] = *(i64 *)&(core->memory[core->U[instruction.register_2] + core->U[instruction.register_3] + instruction.offset]);
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

void loadf(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant LOADF--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %f\n"
                "Le registre %d a pour valeur %f\n"
                "Le registre %d a pour valeur %f\n"
                "core->memory[**address**] a pour valeur %f\n",
                instruction.register_1, core->F[instruction.register_1],
                instruction.register_2, core->F[instruction.register_2],
                instruction.register_3, core->F[instruction.register_3],
                *(double *)&(core->memory[core->U[instruction.register_2] + core->U[instruction.register_3] + instruction.offset]));
    core->F[instruction.register_1] = *(double *)&(core->memory[core->U[instruction.register_2] + core->U[instruction.register_3] + instruction.offset]);
    DEBUG_PRINT("--------Après LOADF--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %f\n"
                "Le registre %d a pour valeur %f\n"
                "Le registre %d a pour valeur %f\n"
                "core->memory[**address**] a pour valeur %f\n\n",
                instruction.register_1, core->F[instruction.register_1],
                instruction.register_2, core->F[instruction.register_2],
                instruction.register_3, core->F[instruction.register_3],
                *(double *)&(core->memory[core->U[instruction.register_2] + core->U[instruction.register_3] + instruction.offset]));
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}
void loadv(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));

    memcpy(&core->V[instruction.register_1], &core->memory[core->U[instruction.register_2] + core->U[instruction.register_3] + instruction.offset], sizeof(u64) * 8);
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}
void loadt(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));

    memcpy(&core->T[instruction.register_1], &core->memory[core->U[instruction.register_2] + core->U[instruction.register_3] + instruction.offset], sizeof(i64) * 8);
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

void loadg(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));

    memcpy(&core->G[instruction.register_1], &core->memory[core->U[instruction.register_2] + core->U[instruction.register_3] + instruction.offset], sizeof(double) * 8);
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}
