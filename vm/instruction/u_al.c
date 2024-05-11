#include "../instruction.h"

void addu(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant Addition--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n",
                instruction.register_1, core->U[instruction.register_1],
                instruction.register_2, core->U[instruction.register_2],
                instruction.register_3, core->U[instruction.register_3]);
    core->U[instruction.register_1] = core->U[instruction.register_2] + core->U[instruction.register_3];
    DEBUG_PRINT("--------Après Addition--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n\n",
                instruction.register_1, core->U[instruction.register_1],
                instruction.register_2, core->U[instruction.register_2],
                instruction.register_3, core->U[instruction.register_3]);
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

void subu(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant Soustraction--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n",
                instruction.register_1, core->U[instruction.register_1],
                instruction.register_2, core->U[instruction.register_2],
                instruction.register_3, core->U[instruction.register_3]);
    core->U[instruction.register_1] = (core->U[instruction.register_2] - core->U[instruction.register_3]);
    DEBUG_PRINT("--------Après Soustraction--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n\n",
                instruction.register_1, core->U[instruction.register_1],
                instruction.register_2, core->U[instruction.register_2],
                instruction.register_3, core->U[instruction.register_3]);

    core->CF[1] = (core->U[instruction.register_2] < core->U[instruction.register_3]) ? true : false;
    core->CF[0] = (core->U[instruction.register_1] == 0) ? true : false;
    
    DEBUG_PRINT("Compare Flag 0 a pour valeur %d\n", core->CF[0]);
    DEBUG_PRINT("Compare Flag 1 a pour valeur %d\n", core->CF[1]);
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

void mulu(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant Multiplication--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n",
                instruction.register_1, core->U[instruction.register_1],
                instruction.register_2, core->U[instruction.register_2],
                instruction.register_3, core->U[instruction.register_3]);
    core->U[instruction.register_1] = core->U[instruction.register_2] * core->U[instruction.register_3];
    DEBUG_PRINT("--------Après Multiplication--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n\n",
                instruction.register_1, core->U[instruction.register_1],
                instruction.register_2, core->U[instruction.register_2],
                instruction.register_3, core->U[instruction.register_3]);
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

void divu(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant Division--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n",
                instruction.register_1, core->U[instruction.register_1],
                instruction.register_2, core->U[instruction.register_2],
                instruction.register_3, core->U[instruction.register_3]);

    // TODO : si division par 0 alors u0 = 0 mais est-ce qu'on mets pas plutôt une erreur ?
    core->CF[1] = (core->U[instruction.register_3] == 0) ? true : false;

    core->U[instruction.register_1] = (core->CF[1] == true) ? 0 : (core->U[instruction.register_2] / core->U[instruction.register_3]);

    core->CF[0] = (core->U[instruction.register_1] == 0) ? true : false;

    DEBUG_PRINT("--------Après Division--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n\n",
                instruction.register_1, core->U[instruction.register_1],
                instruction.register_2, core->U[instruction.register_2],
                instruction.register_3, core->U[instruction.register_3]);
    DEBUG_PRINT("Compare Flag 0 a pour valeur %d\n", core->CF[0]);
    DEBUG_PRINT("Compare Flag 1 a pour valeur %d\n", core->CF[1]);
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

void fmau(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant FMA--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n",
                instruction.register_1, core->U[instruction.register_1],
                instruction.register_2, core->U[instruction.register_2],
                instruction.register_3, core->U[instruction.register_3]);
    core->U[instruction.register_1] += core->U[instruction.register_2] * core->U[instruction.register_3];
    DEBUG_PRINT("--------Avant FMA--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n",
                instruction.register_1, core->U[instruction.register_1],
                instruction.register_2, core->U[instruction.register_2],
                instruction.register_3, core->U[instruction.register_3]);
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

void incu(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant Incrémentation--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n",
                instruction.register_1,
                core->U[instruction.register_1]);
    core->U[instruction.register_1]++;
    DEBUG_PRINT("--------Avant Incrémentation--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n\n",
                instruction.register_1,
                core->U[instruction.register_1]);

    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

void cmpu(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));

    u64 U0 = core->U[instruction.register_1];
    u64 U1 = core->U[instruction.register_2];
    DEBUG_PRINT("Le registre 1 a pour valeur: %ld\n"
                "Le registre 2 a pour valeur: %ld\n",
                U0, U1);

    DEBUG_PRINT("--------Avant Comparaison--------\n");
    for (int i = 7; i > 0; i--)
        DEBUG_PRINT("Compare Flag %d a pour valeur %d\n", i, core->CF[i]);
    core->CF[7] = (U0 == U1);
    core->CF[6] = (U0 != U1);
    core->CF[5] = (U0 > U1);
    core->CF[4] = (U0 >= U1);
    core->CF[3] = (U0 < U1);
    core->CF[2] = (U0 <= U1);
    DEBUG_PRINT("--------Après Comparaison--------\n");
    for (int i = 7; i > 0; i--)
        DEBUG_PRINT("Compare Flag %d a pour valeur %d\n", i, core->CF[i]);
    DEBUG_PRINT("\n");
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}