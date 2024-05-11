#include "../vm.h"
#include "../instruction.h"

void movu(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant Mouvement--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n",
                instruction.register_1,
                core->U[instruction.register_1],
                instruction.register_2, core->U[instruction.register_2]);
    core->U[instruction.register_1] = core->U[instruction.register_2];
    DEBUG_PRINT("--------Après Mouvement--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n\n",
                instruction.register_1,
                core->U[instruction.register_1],
                instruction.register_2, core->U[instruction.register_2]);
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

void movui(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant MOVUI--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n", instruction.register_1,
                core->U[instruction.register_1]);
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
    u64 immediate = get_immediate(core);
    core->U[instruction.register_1] = immediate;
    DEBUG_PRINT("--------Après MOVUI--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n\n", instruction.register_1,
                core->U[instruction.register_1]);
}

void movs(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    core->S[instruction.register_1] = core->S[instruction.register_2];
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

void movsi(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant MOVSI--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %li\n", instruction.register_1,
                core->S[instruction.register_1]);
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
    u64 immediate = get_immediate(core);
    core->S[instruction.register_1] = *(i64 *)&immediate;
    DEBUG_PRINT("--------Avant MOVSI--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %li\n\n", instruction.register_1,
                core->S[instruction.register_1]);
}

void movf(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant Mouvement Flottant--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %f\n"
                "Le registre %d a pour valeur %f\n",
                instruction.register_1,
                core->F[instruction.register_1],
                instruction.register_2,
                core->F[instruction.register_2]);
    core->F[instruction.register_1] = core->F[instruction.register_2];
    DEBUG_PRINT("--------Après Mouvement Flottant--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %f\n"
                "Le registre %d a pour valeur %f\n\n",
                instruction.register_1,
                core->F[instruction.register_1],
                instruction.register_2,
                core->F[instruction.register_2]);
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

void movfi(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant MOVFI--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %lf\n", instruction.register_1,
                core->F[instruction.register_1]);
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
    u64 immediate = get_immediate(core);
    core->F[instruction.register_1] = *(f64 *)&immediate;
    DEBUG_PRINT("--------Après MOVFI--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %lf\n\n", instruction.register_1,
                core->F[instruction.register_1]);
}
