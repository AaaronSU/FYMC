#include "../vm.h"
#include "../instruction.h"
#include <math.h>

void adds(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant Addition--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n",
                instruction.register_1, core->S[instruction.register_1],
                instruction.register_2, core->S[instruction.register_2],
                instruction.register_3, core->S[instruction.register_3]);
    core->S[instruction.register_1] = core->S[instruction.register_2] + core->S[instruction.register_3];
    DEBUG_PRINT("--------Après Addition--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n\n",
                instruction.register_1, core->S[instruction.register_1],
                instruction.register_2, core->S[instruction.register_2],
                instruction.register_3, core->S[instruction.register_3]);
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

//
void subs(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant Soustraction--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n",
                instruction.register_1, core->S[instruction.register_1],
                instruction.register_2, core->S[instruction.register_2],
                instruction.register_3, core->S[instruction.register_3]);
    core->S[instruction.register_1] = (core->S[instruction.register_2] - core->S[instruction.register_3]);
    DEBUG_PRINT("--------Après Soustraction--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n\n",
                instruction.register_1, core->S[instruction.register_1],
                instruction.register_2, core->S[instruction.register_2],
                instruction.register_3, core->S[instruction.register_3]);

    core->CF[1] = (core->S[instruction.register_2] < core->S[instruction.register_3]) ? true : false;
    core->CF[0] = (core->S[instruction.register_1] == 0) ? true : false;

    DEBUG_PRINT("Compare Flag 0 a pour valeur %d\n", core->CF[0]);
    DEBUG_PRINT("Compare Flag 1 a pour valeur %d\n", core->CF[1]);
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

//
void muls(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant Multiplication--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n",
                instruction.register_1, core->S[instruction.register_1],
                instruction.register_2, core->S[instruction.register_2],
                instruction.register_3, core->S[instruction.register_3]);
    core->S[instruction.register_1] = core->S[instruction.register_2] * core->S[instruction.register_3];
    DEBUG_PRINT("--------Après Multiplication--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n\n",
                instruction.register_1, core->S[instruction.register_1],
                instruction.register_2, core->S[instruction.register_2],
                instruction.register_3, core->S[instruction.register_3]);
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

//
void divs(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant Division--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n",
                instruction.register_1, core->S[instruction.register_1],
                instruction.register_2, core->S[instruction.register_2],
                instruction.register_3, core->S[instruction.register_3]);

    // TODO : si division par 0 alors u0 = 0 mais est-ce qu'on mets pas plutôt une erreur ?
    core->CF[1] = (core->S[instruction.register_3] == 0) ? true : false;

    core->S[instruction.register_1] = (core->CF[1] == true) ? 0 : (core->S[instruction.register_2] / core->S[instruction.register_3]);

    core->CF[0] = (core->S[instruction.register_1] == 0) ? true : false;

    DEBUG_PRINT("--------Après Division--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n\n",
                instruction.register_1, core->S[instruction.register_1],
                instruction.register_2, core->S[instruction.register_2],
                instruction.register_3, core->S[instruction.register_3]);
    DEBUG_PRINT("Compare Flag 0 a pour valeur %d\n", core->CF[0]);
    DEBUG_PRINT("Compare Flag 1 a pour valeur %d\n", core->CF[1]);
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

//
void mods(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant Modulo--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n",
                instruction.register_1, core->S[instruction.register_1],
                instruction.register_2, core->S[instruction.register_2],
                instruction.register_3, core->S[instruction.register_3]);

    // TODO : si modulo par 0 alors u0 = 0 mais est-ce qu'on mets pas plutôt une erreur ?
    core->CF[1] = (core->S[instruction.register_3] == 0) ? true : false;

    core->S[instruction.register_1] = (core->CF[1] == true) ? 0 : (core->S[instruction.register_2] % core->S[instruction.register_3]);

    core->CF[0] = (core->S[instruction.register_1] == 0) ? true : false;

    DEBUG_PRINT("--------Après Modulo--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n\n",
                instruction.register_1, core->S[instruction.register_1],
                instruction.register_2, core->S[instruction.register_2],
                instruction.register_3, core->S[instruction.register_3]);
    DEBUG_PRINT("Compare Flag 0 a pour valeur %d\n", core->CF[0]);
    DEBUG_PRINT("Compare Flag 1 a pour valeur %d\n", core->CF[1]);
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

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

//
void sqrts(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant Racine U--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n",
                instruction.register_1, core->S[instruction.register_1],
                instruction.register_2, core->S[instruction.register_2]);

    core->S[instruction.register_1] = (i64)sqrt((f64)core->S[instruction.register_2]);

    core->CF[0] = (core->S[instruction.register_1] == 0) ? true : false;

    DEBUG_PRINT("--------Après Racine U--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n",
                instruction.register_1, core->S[instruction.register_1],
                instruction.register_2, core->S[instruction.register_2]);
    DEBUG_PRINT("Compare Flag 0 a pour valeur %d\n", core->CF[0]);
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

//
void logs(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant log -------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n",
                instruction.register_1, core->S[instruction.register_1],
                instruction.register_2, core->S[instruction.register_2]);

    if (core->S[instruction.register_2] == 0)
    {
        printf("log(0) no.\n");
        exit(EXIT_FAILURE);
    }

    core->S[instruction.register_1] = (i64)log10((f64)core->S[instruction.register_2]);

    core->CF[0] = (core->S[instruction.register_1] == 0) ? true : false;

    DEBUG_PRINT("--------Après log--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n",
                instruction.register_1, core->S[instruction.register_1],
                instruction.register_2, core->S[instruction.register_2]);
    DEBUG_PRINT("Compare Flag 0 a pour valeur %d\n", core->CF[0]);
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

//
void ands(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant ANDU--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n",
                instruction.register_1, core->S[instruction.register_1],
                instruction.register_2, core->S[instruction.register_2],
                instruction.register_3, core->S[instruction.register_3]);

    core->S[instruction.register_1] = (core->S[instruction.register_2] & core->S[instruction.register_3]);
    DEBUG_PRINT("--------Après ANDU--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n\n",
                instruction.register_1, core->S[instruction.register_1],
                instruction.register_2, core->S[instruction.register_2],
                instruction.register_3, core->S[instruction.register_3]);

    core->CF[0] = (core->S[instruction.register_1] == 0) ? true : false;

    DEBUG_PRINT("Compare Flag 0 a pour valeur %d\n", core->CF[0]);

    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

//

void ors(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant ORU--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n",
                instruction.register_1, core->S[instruction.register_1],
                instruction.register_2, core->S[instruction.register_2],
                instruction.register_3, core->S[instruction.register_3]);

    core->S[instruction.register_1] = (core->S[instruction.register_2] | core->S[instruction.register_3]);
    DEBUG_PRINT("--------Après ORU--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n\n",
                instruction.register_1, core->S[instruction.register_1],
                instruction.register_2, core->S[instruction.register_2],
                instruction.register_3, core->S[instruction.register_3]);

    core->CF[0] = (core->S[instruction.register_1] == 0) ? true : false;

    DEBUG_PRINT("Compare Flag 0 a pour valeur %d\n", core->CF[0]);

    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

void xors(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant XORU--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n",
                instruction.register_1, core->S[instruction.register_1],
                instruction.register_2, core->S[instruction.register_2],
                instruction.register_3, core->S[instruction.register_3]);

    core->S[instruction.register_1] = (core->S[instruction.register_2] ^ core->S[instruction.register_3]);
    DEBUG_PRINT("--------Après XORU--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n\n",
                instruction.register_1, core->S[instruction.register_1],
                instruction.register_2, core->S[instruction.register_2],
                instruction.register_3, core->S[instruction.register_3]);

    core->CF[0] = (core->S[instruction.register_1] == 0) ? true : false;

    DEBUG_PRINT("Compare Flag 0 a pour valeur %d\n", core->CF[0]);

    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}
