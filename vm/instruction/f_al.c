#include "../vm.h"
#include "../instruction.h"

#include <math.h>
#include "../instruction.h"

void addf(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant Addition--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %f\n"
                "Le registre %d a pour valeur %f\n"
                "Le registre %d a pour valeur %f\n",
                instruction.register_1, core->F[instruction.register_1],
                instruction.register_2, core->F[instruction.register_2],
                instruction.register_3, core->F[instruction.register_3]);
    core->F[instruction.register_1] = core->F[instruction.register_2] + core->F[instruction.register_3];
    DEBUG_PRINT("--------Après Addition--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %f\n"
                "Le registre %d a pour valeur %f\n"
                "Le registre %d a pour valeur %f\n\n",
                instruction.register_1, core->F[instruction.register_1],
                instruction.register_2, core->F[instruction.register_2],
                instruction.register_3, core->F[instruction.register_3]);
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

//
void subf(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant Soustraction--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %f\n"
                "Le registre %d a pour valeur %f\n"
                "Le registre %d a pour valeur %f\n",
                instruction.register_1, core->F[instruction.register_1],
                instruction.register_2, core->F[instruction.register_2],
                instruction.register_3, core->F[instruction.register_3]);
    core->F[instruction.register_1] = (core->F[instruction.register_2] - core->F[instruction.register_3]);
    DEBUG_PRINT("--------Après Soustraction--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %f\n"
                "Le registre %d a pour valeur %f\n"
                "Le registre %d a pour valeur %f\n\n",
                instruction.register_1, core->F[instruction.register_1],
                instruction.register_2, core->F[instruction.register_2],
                instruction.register_3, core->F[instruction.register_3]);

    core->CF[1] = (core->F[instruction.register_2] < core->F[instruction.register_3]) ? true : false;
    core->CF[0] = (core->F[instruction.register_1] == 0) ? true : false;

    DEBUG_PRINT("Compare Flag 0 a pour valeur %d\n", core->CF[0]);
    DEBUG_PRINT("Compare Flag 1 a pour valeur %d\n", core->CF[1]);
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

//
void mulf(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant Multiplication--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %f\n"
                "Le registre %d a pour valeur %f\n"
                "Le registre %d a pour valeur %f\n",
                instruction.register_1, core->F[instruction.register_1],
                instruction.register_2, core->F[instruction.register_2],
                instruction.register_3, core->F[instruction.register_3]);
    core->F[instruction.register_1] = core->F[instruction.register_2] * core->F[instruction.register_3];
    DEBUG_PRINT("--------Après Multiplication--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %f\n"
                "Le registre %d a pour valeur %f\n"
                "Le registre %d a pour valeur %f\n\n",
                instruction.register_1, core->F[instruction.register_1],
                instruction.register_2, core->F[instruction.register_2],
                instruction.register_3, core->F[instruction.register_3]);
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

//
void divf(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant Division--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %f\n"
                "Le registre %d a pour valeur %f\n"
                "Le registre %d a pour valeur %f\n",
                instruction.register_1, core->F[instruction.register_1],
                instruction.register_2, core->F[instruction.register_2],
                instruction.register_3, core->F[instruction.register_3]);

    // TODO : si division par 0 alors u0 = 0 mais est-ce qu'on mets pas plutôt une erreur ?
    core->CF[1] = (core->F[instruction.register_3] == 0) ? true : false;

    core->F[instruction.register_1] = (core->CF[1] == true) ? 0 : (core->F[instruction.register_2] / core->F[instruction.register_3]);

    core->CF[0] = (core->F[instruction.register_1] == 0) ? true : false;

    DEBUG_PRINT("--------Après Division--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %f\n"
                "Le registre %d a pour valeur %f\n"
                "Le registre %d a pour valeur %f\n\n",
                instruction.register_1, core->F[instruction.register_1],
                instruction.register_2, core->F[instruction.register_2],
                instruction.register_3, core->F[instruction.register_3]);
    DEBUG_PRINT("Compare Flag 0 a pour valeur %d\n", core->CF[0]);
    DEBUG_PRINT("Compare Flag 1 a pour valeur %d\n", core->CF[1]);
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

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

//
void sqrtf_(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant Racine U--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %f\n"
                "Le registre %d a pour valeur %f\n",
                instruction.register_1, core->F[instruction.register_1],
                instruction.register_2, core->F[instruction.register_2]);

    core->F[instruction.register_1] = sqrt(core->F[instruction.register_2]);

    core->CF[0] = (core->F[instruction.register_1] == 0) ? true : false;

    DEBUG_PRINT("--------Après Racine U--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %f\n"
                "Le registre %d a pour valeur %f\n",
                instruction.register_1, core->F[instruction.register_1],
                instruction.register_2, core->F[instruction.register_2]);
    DEBUG_PRINT("Compare Flag 0 a pour valeur %d\n", core->CF[0]);
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

//
void logf_(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant log -------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %f\n"
                "Le registre %d a pour valeur %f\n",
                instruction.register_1, core->F[instruction.register_1],
                instruction.register_2, core->F[instruction.register_2]);

    if (core->F[instruction.register_2] == 0)
    {
        printf("log(0) no.\n");
        exit(EXIT_FAILURE);
    }

    core->F[instruction.register_1] = log10(core->F[instruction.register_2]);

    core->CF[0] = (core->F[instruction.register_1] == 0) ? true : false;

    DEBUG_PRINT("--------Après log--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %f\n"
                "Le registre %d a pour valeur %f\n",
                instruction.register_1, core->F[instruction.register_1],
                instruction.register_2, core->F[instruction.register_2]);
    DEBUG_PRINT("Compare Flag 0 a pour valeur %d\n", core->CF[0]);
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}
