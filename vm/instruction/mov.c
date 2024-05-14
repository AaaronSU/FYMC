#include "../vm.h"
#include "../instruction.h"

void mov(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    u8 type_register = instruction.offset & 0x10;
    u8 r1 = instruction.offset & 0xC;
    u8 r2 = instruction.offset & 0x3;

    switch (type_register)
    {
    case 0:
        // scalaire
        // U --> 00 S --> 01 F --> 10
        switch (r1)
        {
        case 0:
            switch (r2)
            {
            case 0:
                core->U[instruction.register_1] = core->U[instruction.register_2];
                break;
            case 1:
                core->U[instruction.register_1] = (u64)core->S[instruction.register_2];
                break;
            case 2:
                core->U[instruction.register_1] = (u64)core->F[instruction.register_2];
                break;
            }
            break;

        case 1:
            switch (r2)
            {
            case 0:
                core->S[instruction.register_1] = (i64)core->U[instruction.register_2];
                break;
            case 1:
                core->S[instruction.register_1] = core->S[instruction.register_2];
                break;
            case 2:
                core->S[instruction.register_1] = (i64)core->F[instruction.register_2];
                break;
            }
            break;

        case 2:
            switch (r2)
            {
            case 0:
                core->F[instruction.register_1] = (f64)core->U[instruction.register_2];
                break;
            case 1:
                core->F[instruction.register_1] = (f64)core->S[instruction.register_2];
                break;
            case 2:
                core->F[instruction.register_1] = core->F[instruction.register_2];
                break;
            }
            break;
        }
        break;

    case 1:
        // vecteur
        // V --> 00 T --> 01 G --> 10
        switch (r1)
        {
        case 0:
            switch (r2)
            {
            case 0:
                for (int i = 0; i < NUMBER_SCALAR_IN_VECTOR_REGISTER; ++i)
                {
                    core->V[instruction.register_1][i] = core->V[instruction.register_2][i];
                }
                break;
            case 1:
                for (int i = 0; i < NUMBER_SCALAR_IN_VECTOR_REGISTER; ++i)
                {
                    core->V[instruction.register_1][i] = (u64)core->T[instruction.register_2][i];
                }
                break;
            case 2:
                for (int i = 0; i < NUMBER_SCALAR_IN_VECTOR_REGISTER; ++i)
                {
                    core->V[instruction.register_1][i] = (u64)core->G[instruction.register_2][i];
                }
                break;
            }
            break;

        case 1:
            switch (r2)
            {
            case 0:
                for (int i = 0; i < NUMBER_SCALAR_IN_VECTOR_REGISTER; ++i)
                {
                    core->T[instruction.register_1][i] = (i64)core->V[instruction.register_2][i];
                }
                break;
            case 1:
                for (int i = 0; i < NUMBER_SCALAR_IN_VECTOR_REGISTER; ++i)
                {
                    core->T[instruction.register_1][i] = core->T[instruction.register_2][i];
                }
                break;
            case 2:
                for (int i = 0; i < NUMBER_SCALAR_IN_VECTOR_REGISTER; ++i)
                {
                    core->T[instruction.register_1][i] = (i64)core->G[instruction.register_2][i];
                }
                break;
            }
            break;

        case 2:
            switch (r2)
            {
            case 0:
                for (int i = 0; i < NUMBER_SCALAR_IN_VECTOR_REGISTER; ++i)
                {
                    core->G[instruction.register_1][i] = (f64)core->V[instruction.register_2][i];
                }
                break;
            case 1:
                for (int i = 0; i < NUMBER_SCALAR_IN_VECTOR_REGISTER; ++i)
                {
                    core->G[instruction.register_1][i] = (f64)core->T[instruction.register_2][i];
                }
                break;
            case 2:
                for (int i = 0; i < NUMBER_SCALAR_IN_VECTOR_REGISTER; ++i)
                {
                    core->G[instruction.register_1][i] = core->G[instruction.register_2][i];
                }
                break;
            }
            break;
        }
        break;

    default:
        printf("Erreur: Type de registre non valide\n");
        exit(EXIT_FAILURE);
        break;
    }

    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

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

void movs(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    core->S[instruction.register_1] = core->S[instruction.register_2];
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
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

void movv(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
    for (u8 indice = 0; indice < NUMBER_SCALAR_IN_VECTOR_REGISTER; indice++)
    {
        core->V[instruction.register_1][indice] = core->V[instruction.register_2][indice];
    }
}

void movt(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
    for (u8 indice = 0; indice < NUMBER_SCALAR_IN_VECTOR_REGISTER; indice++)
    {
        core->T[instruction.register_1][indice] = core->T[instruction.register_2][indice];
    }
}

void movg(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
    for (u8 indice = 0; indice < NUMBER_SCALAR_IN_VECTOR_REGISTER; indice++)
    {
        core->G[instruction.register_1][indice] = core->G[instruction.register_2][indice];
    }
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

void movfi(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    DEBUG_PRINT("--------Avant MOVFI--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %lf\n", instruction.register_1,
                core->F[instruction.register_1]);
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
    u64 immediate = get_immediate(core);
    f64 immediate_value;
    memcpy(&immediate_value, &immediate, sizeof(f64));
    core->F[instruction.register_1] = immediate_value;
    DEBUG_PRINT("--------Après MOVFI--------\n");
    DEBUG_PRINT("Le registre %d a pour valeur %lf\n\n", instruction.register_1,
                core->F[instruction.register_1]);
}

void movvi(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
    for (u8 indice = 0; indice < NUMBER_SCALAR_IN_VECTOR_REGISTER; indice++)
    {
        u64 immediate = get_immediate(core);
        core->V[instruction.register_1][indice] = immediate;
    }
}

void movti(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
    for (u8 indice = 0; indice < NUMBER_SCALAR_IN_VECTOR_REGISTER; indice++)
    {
        u64 immediate = get_immediate(core);
        i64 immediate_value;
        memcpy(&immediate_value, &immediate, sizeof(i64));
        core->T[instruction.register_1][indice] = immediate_value;
    }
}

void movgi(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
    for (u8 indice = 0; indice < NUMBER_SCALAR_IN_VECTOR_REGISTER; indice++)
    {
        u64 immediate = get_immediate(core);
        f64 immediate_value;
        memcpy(&immediate_value, &immediate, sizeof(f64));
        core->G[instruction.register_1][indice] = immediate_value;
    }
}