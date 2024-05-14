#include <math.h>
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

//
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

//
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

//
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

//
void modu(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant Modulo--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n",
                instruction.register_1, core->U[instruction.register_1],
                instruction.register_2, core->U[instruction.register_2],
                instruction.register_3, core->U[instruction.register_3]);

    // TODO : si modulo par 0 alors u0 = 0 mais est-ce qu'on mets pas plutôt une erreur ?
    core->CF[1] = (core->U[instruction.register_3] == 0) ? true : false;

    core->U[instruction.register_1] = (core->CF[1] == true) ? 0 : (core->U[instruction.register_2] % core->U[instruction.register_3]);

    core->CF[0] = (core->U[instruction.register_1] == 0) ? true : false;

    DEBUG_PRINT("--------Après Modulo--------\n");
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

//
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

//
void sqrtu(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant Racine U--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n",
                instruction.register_1, core->U[instruction.register_1],
                instruction.register_2, core->U[instruction.register_2]);

    core->U[instruction.register_1] = (u64)sqrt((f64)core->U[instruction.register_2]);

    core->CF[0] = (core->U[instruction.register_1] == 0) ? true : false;

    DEBUG_PRINT("--------Après Racine U--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n",
                instruction.register_1, core->U[instruction.register_1],
                instruction.register_2, core->U[instruction.register_2]);
    DEBUG_PRINT("Compare Flag 0 a pour valeur %d\n", core->CF[0]);
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

//
void logu(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant log -------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n",
                instruction.register_1, core->U[instruction.register_1],
                instruction.register_2, core->U[instruction.register_2]);

    if (core->U[instruction.register_2] == 0)
    {
        printf("log(0) no.\n");
        exit(EXIT_FAILURE);
    }

    core->U[instruction.register_1] = (u64)log10((f64)core->U[instruction.register_2]);

    core->CF[0] = (core->U[instruction.register_1] == 0) ? true : false;

    DEBUG_PRINT("--------Après log--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n",
                instruction.register_1, core->U[instruction.register_1],
                instruction.register_2, core->U[instruction.register_2]);
    DEBUG_PRINT("Compare Flag 0 a pour valeur %d\n", core->CF[0]);
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

//
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

//
void decu(core_t *core)
{

    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant Décrémentation--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n",
                instruction.register_1,
                core->U[instruction.register_1]);

    core->CF[1] = (core->U[instruction.register_1] == 0) ? true : false;

    core->U[instruction.register_1]--;

    core->CF[0] = (core->U[instruction.register_1] == 0) ? true : false;

    DEBUG_PRINT("--------Avant Décrémentation--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n\n",
                instruction.register_1,
                core->U[instruction.register_1]);

    DEBUG_PRINT("Compare Flag 0 a pour valeur %d\n", core->CF[0]);
    DEBUG_PRINT("Compare Flag 1 a pour valeur %d\n", core->CF[1]);

    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

//
void andu(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant ANDU--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n",
                instruction.register_1, core->U[instruction.register_1],
                instruction.register_2, core->U[instruction.register_2],
                instruction.register_3, core->U[instruction.register_3]);

    core->U[instruction.register_1] = (core->U[instruction.register_2] & core->U[instruction.register_3]);
    DEBUG_PRINT("--------Après ANDU--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n\n",
                instruction.register_1, core->U[instruction.register_1],
                instruction.register_2, core->U[instruction.register_2],
                instruction.register_3, core->U[instruction.register_3]);

    core->CF[0] = (core->U[instruction.register_1] == 0) ? true : false;

    DEBUG_PRINT("Compare Flag 0 a pour valeur %d\n", core->CF[0]);

    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

//

void oru(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant ORU--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n",
                instruction.register_1, core->U[instruction.register_1],
                instruction.register_2, core->U[instruction.register_2],
                instruction.register_3, core->U[instruction.register_3]);

    core->U[instruction.register_1] = (core->U[instruction.register_2] | core->U[instruction.register_3]);
    DEBUG_PRINT("--------Après ORU--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n\n",
                instruction.register_1, core->U[instruction.register_1],
                instruction.register_2, core->U[instruction.register_2],
                instruction.register_3, core->U[instruction.register_3]);

    core->CF[0] = (core->U[instruction.register_1] == 0) ? true : false;

    DEBUG_PRINT("Compare Flag 0 a pour valeur %d\n", core->CF[0]);

    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

void xoru(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant XORU--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n",
                instruction.register_1, core->U[instruction.register_1],
                instruction.register_2, core->U[instruction.register_2],
                instruction.register_3, core->U[instruction.register_3]);

    core->U[instruction.register_1] = (core->U[instruction.register_2] ^ core->U[instruction.register_3]);
    DEBUG_PRINT("--------Après XORU--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n"
                "Le registre %d a pour valeur %ld\n\n",
                instruction.register_1, core->U[instruction.register_1],
                instruction.register_2, core->U[instruction.register_2],
                instruction.register_3, core->U[instruction.register_3]);

    core->CF[0] = (core->U[instruction.register_1] == 0) ? true : false;

    DEBUG_PRINT("Compare Flag 0 a pour valeur %d\n", core->CF[0]);

    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

void shlu(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));

    core->U[instruction.register_1] = (core->U[instruction.register_2] << core->U[instruction.register_3]);
    core->CF[0] = (core->U[instruction.register_1] == 0) ? true : false;

    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

void shru(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));

    core->U[instruction.register_1] = (core->U[instruction.register_2] >> core->U[instruction.register_3]);
    core->CF[0] = (core->U[instruction.register_1] == 0) ? true : false;

    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

void rolu(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));

    core->U[instruction.register_1] = (core->U[instruction.register_2] << core->U[instruction.register_3]) |
                                      (core->U[instruction.register_2] >> core->U[instruction.register_3]);
    core->CF[0] = (core->U[instruction.register_1] == 0) ? true : false;

    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

void roru(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));

    core->U[instruction.register_1] = (core->U[instruction.register_2] >> core->U[instruction.register_3]) |
                                      (core->U[instruction.register_2] << core->U[instruction.register_3]);
    core->CF[0] = (core->U[instruction.register_1] == 0) ? true : false;

    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

void popcntu(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));

    u64 count = 0;
    u64 value = core->U[instruction.register_2];
    while (value != 0)
    {
        count += value & 1;
        value >>= 1;
    }
    core->U[instruction.register_1] = count;

    core->CF[0] = (core->U[instruction.register_1] == 0) ? true : false;

    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

void lmbu(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));

    u64 position = 0;
    u64 value = core->U[instruction.register_2];
    while (value != 0)
    {
        position++;
        value >>= 1;
    }
    core->U[instruction.register_1] = position;

    core->CF[0] = (core->U[instruction.register_1] == 0) ? true : false;

    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}
