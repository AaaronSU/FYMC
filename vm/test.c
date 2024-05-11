#include <stdint.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "instruction.h"

#define MAX_ITERATION 1000
#define MEMORY_SIZE 1024 * 1024

//
static core_t *core_init()
{
    core_t *core = (core_t *)malloc(sizeof(core_t));
    if (core == NULL)
    {
        printf("failed to allocate core of size %zu bytes", sizeof(core_t));
        exit(1);
    }

    for (int i = 0; i < NUMBER_SCALAR_REGISTER; ++i)
    {
        core->U[i] = 0;
        core->S[i] = 0;
        core->F[i] = 0.0;
    }

    for (int i = 0; i < 8; i++)
    {
        core->CF[i] = false;
    }

    core->IP = 0;

    return core;
}

//
static u32 create_instruction(u8 opcode, u16 offset, u8 register_1, u8 register_2, u8 register_3)
{
    return htobe32((opcode & 0xFF) << (SIZE_INSTRUCTION - 8) | (offset & 0x1FF) << (SIZE_INSTRUCTION - 17) | (register_1 & 0x1F) << (SIZE_INSTRUCTION - 22) | (register_2 & 0x1F) << (SIZE_INSTRUCTION - 27) | (register_3 & 0x1F));
}

// u1 = memory(u2 + u3 + offset)
static void test_loadu(void **state)
{
    (void) state;
    core_t *core = core_init();

    size_t file_size = sizeof(u32);
    char *file_buffer = (char *)malloc(file_size);

    if (file_buffer == NULL)
    {
        printf("failed to allocate file buffer of size %lu bytes", file_size);
        free(core);
        exit(1);
    }

    core->file_buffer = file_buffer;

    u8 *memory = (u8 *)malloc(MEMORY_SIZE);
    if (memory == NULL)
    {
        printf("failed to allocate memory of size %u bytes", MEMORY_SIZE);
        free(file_buffer);
        free(core);
        exit(1);
    }

    core->memory = memory;

    for (int i = 0; i < MAX_ITERATION; ++i)
    {
        // intialisation des registres
        u8 r1 = (u8)(rand() % 10);        // entre 0 et 9
        u8 r2 = (u8)(10 + (rand() % 10)); // entre 10 et 19
        u8 r3 = (u8)(20 + (rand() % 12)); // entre 20 et 31

        core->U[r2] = (u64)(rand() % (MEMORY_SIZE / 3));
        core->U[r3] = (u64)(rand() % (MEMORY_SIZE / 3));
        u8 offset = (u8)rand();

        u32 *ptr = (u32 *)(core->file_buffer + core->IP);
        *ptr = create_instruction(0, offset, r1, r2, r3);

        u64 value = (u64)rand();
        u64 indice = core->U[r2] + core->U[r3] + offset;

        *(u64 *)(core->memory + indice) = value;

        loadu(core);
        core->IP = 0;

        assert_int_equal(core->U[r1], value);
    }

    core_drop(core);
}

// instruction_set[1] = loads;

// f1 = memory(u2 + u3 + offset)
static void test_loadf(void **state)
{
    (void)state;
    core_t *core = core_init();

    size_t file_size = sizeof(u32);
    char *file_buffer = (char *)malloc(file_size);

    if (file_buffer == NULL)
    {
        printf("failed to allocate file buffer of size %lu bytes", file_size);
        free(core);
        exit(1);
    }

    core->file_buffer = file_buffer;

    u8 *memory = (u8 *)malloc(MEMORY_SIZE);
    if (memory == NULL)
    {
        printf("failed to allocate memory of size %u bytes", MEMORY_SIZE);
        free(file_buffer);
        free(core);
        exit(1);
    }

    core->memory = memory;

    for (int i = 0; i < MAX_ITERATION; ++i)
    {
        // intialisation des registres
        u8 r1 = (u8)(rand() % 32);        // entre 0 et 32
        u8 r2 = (u8)((rand() % 15));      // entre 0 et 14
        u8 r3 = (u8)(15 + (rand() % 17)); // entre 15 et 31

        core->U[r2] = (u64)(rand() % (MEMORY_SIZE / 3));
        core->U[r3] = (u64)(rand() % (MEMORY_SIZE / 3));
        u8 offset = (u8)rand();

        u32 *ptr = (u32 *)(core->file_buffer);
        *ptr = create_instruction(0, offset, r1, r2, r3);

        f64 value = (f64)(rand() / 2.2814);
        u64 indice = core->U[r2] + core->U[r3] + offset;

        *(f64 *)(core->memory + indice) = value;

        loadf(core);
        core->IP = 0;

        assert_float_equal(core->F[r1], value, 0.0001);
    }

    core_drop(core);
}

// memory(u1 + u2 + offset) = u3
static void test_storeu(void **state)
{
    (void)state;
    core_t *core = core_init();

    size_t file_size = sizeof(u32);
    char *file_buffer = (char *)malloc(file_size);

    if (file_buffer == NULL)
    {
        printf("failed to allocate file buffer of size %lu bytes", file_size);
        free(core);
        exit(1);
    }

    core->file_buffer = file_buffer;

    u8 *memory = (u8 *)malloc(MEMORY_SIZE);
    if (memory == NULL)
    {
        printf("failed to allocate memory of size %u bytes", MEMORY_SIZE);
        free(core);
        free(memory);
        exit(1);
    }

    core->memory = memory;

    for (int i = 0; i < MAX_ITERATION; ++i)
    {
        // intialisation des registres
        u8 r1 = (u8)(rand() % 10);        // entre 0 et 9
        u8 r2 = (u8)(10 + (rand() % 10)); // entre 10 et 19
        u8 r3 = (u8)(20 + (rand() % 12)); // entre 20 et 31
        // indice
        core->U[r1] = (u64)(rand() % (MEMORY_SIZE / 3));
        core->U[r2] = (u64)(rand() % (MEMORY_SIZE / 3));
        u8 offset = (u8)rand();

        // value
        core->U[r3] = (u64)(rand() % (MEMORY_SIZE / 3));
        u32 *ptr = (u32 *)(core->file_buffer + core->IP);
        *ptr = create_instruction(0, offset, r1, r2, r3);

        u64 indice = core->U[r1] + core->U[r2] + offset;

        storeu(core);
        core->IP = 0;

        assert_int_equal(core->U[r3], *(u64 *)(core->memory + indice));
    }

    core_drop(core);
}

// instruction_set[7] = stores;

// memory(u1 + u2 + offset) = f3
static void test_storef(void **state)
{
    (void)state;
    core_t *core = core_init();

    size_t file_size = sizeof(u32);
    char *file_buffer = (char *)malloc(file_size);

    if (file_buffer == NULL)
    {
        printf("failed to allocate file buffer of size %lu bytes", file_size);
        free(core);
        exit(1);
    }

    core->file_buffer = file_buffer;

    u8 *memory = (u8 *)malloc(MEMORY_SIZE);
    if (memory == NULL)
    {
        printf("failed to allocate memory of size %u bytes", MEMORY_SIZE);
        free(core);
        free(memory);
        exit(1);
    }

    core->memory = memory;

    for (int i = 0; i < MAX_ITERATION; ++i)
    {
        // intialisation des registres
        u8 r1 = (u8)(rand() % 15);          // entre 0 et 14
        u8 r2 = (u8)(15 + (rand() % 17));   // entre 15 et 31
        u8 r3 = (u8)(rand() % 32);          // entre 0 et 31

        // indice
        core->U[r1] = (u64)(rand() % (MEMORY_SIZE / 3));
        core->U[r2] = (u64)(rand() % (MEMORY_SIZE / 3));
        u8 offset = (u8)rand();

        // value (taille de la mémoire doit au moins être 8 octets)
        core->F[r3] = (f64)(rand() / 2.1541);

        u32 *ptr = (u32 *)(core->file_buffer);
        *ptr = create_instruction(0, offset, r1, r2, r3);

        u64 indice = core->U[r1] + core->U[r2] + offset;

        storef(core);
        core->IP = 0;

        assert_float_equal(core->F[r3], *(f64 *)(core->memory + indice), 0.0001);
    }

    core_drop(core);
}

// u1 = u2
static void test_movu(void **state)
{
    (void)state;
    core_t *core = core_init();

    size_t file_size = sizeof(u32);
    char *file_buffer = (char *)malloc(file_size);

    if (file_buffer == NULL)
    {
        printf("failed to allocate file buffer of size %lu bytes", file_size);
        free(core);
        exit(1);
    }

    core->file_buffer = file_buffer;

    // U[r1] = U[r2]
    for (int i = 0; i < MAX_ITERATION; ++i)
    {
        u8 r1 = (u8)(random() % 15); // Entre 0 et 14
        u8 r2 = (u8)(15 + random() % 17); // Entre 15 et 31

        u64 v1 = (u64)((rand() % (RAND_MAX / 2)) + (RAND_MAX / 2));
        u64 v2 = (u64)(rand() % (RAND_MAX / 2));

        core->U[r1] = v1;
        core->U[r2] = v2;

        u32 *ptr = (u32 *)(core->file_buffer);
        *ptr = create_instruction(0, 0, r1, r2, 0);

        assert_int_not_equal(v1, v2);

        movu(core);
        core->IP = 0;

        assert_int_equal(core->U[r1], core->U[r2]);
        assert_int_equal(v2, core->U[r2]);
    }

    // cas extreme des u64 ?

    // cas ou c'est pas des u64 ?
    free(file_buffer);
    free(core);
}

// f1 = f2
static void test_movf(void **state)
{
    (void)state;
    core_t *core = core_init();

    size_t file_size = sizeof(u32);
    char *file_buffer = (char *)malloc(file_size);

    if (file_buffer == NULL)
    {
        printf("failed to allocate file buffer of size %lu bytes", file_size);
        free(core);
        exit(1);
    }

    core->file_buffer = file_buffer;

    for (int i = 0; i < 1; ++i)
    {
        u8 r1 = (u8)(random() % 15);      // Entre 0 et 14
        u8 r2 = (u8)(15 + random() % 17); // Entre 15 et 31

        f64 v1 = (f64)((rand() % (RAND_MAX / 2)) + (RAND_MAX / 2)) / 2.2156;
        f64 v2 = (f64)(rand() % (RAND_MAX / 2));

        core->F[r1] = v1;
        core->F[r2] = v2;

        u32 *ptr = (u32 *)(core->file_buffer);
        *ptr = create_instruction(0, 0, r1, r2, 0);

        assert_float_not_equal(v1, v2, 0.0001);

        movf(core);
        core->IP = 0;

        assert_float_equal(core->F[r1], core->F[r2], 0.0001);
        assert_float_equal(v2, core->F[r2], 0.0001);
    }

    // cas extreme des u64 ?

    // cas ou c'est pas des u64 ?
    free(file_buffer);
    free(core);
}

// u1 = imm1
static void test_movui(void **state)
{
    (void)state;
    core_t *core = core_init();

    size_t file_size = sizeof(u32) + sizeof(u64);
    char *file_buffer = (char *)malloc(file_size);

    if (file_buffer == NULL)
    {
        printf("failed to allocate file buffer of size %lu bytes", file_size);
        free(core);
        exit(1);
    }

    core->file_buffer = file_buffer;

    // U[r1] = imm (test quand U[r1] = 0 et U[r1] =! 0 si MAX_ITERATION grand)
    for (int i = 0; i < MAX_ITERATION; ++i)
    {
        u8 r1 = (u8)(random() % 32);
        u64 imm = (u64)random();

        u32 *ptr_inst = (u32 *)(core->file_buffer + core->IP);
        u64 *ptr_imm = (u64 *)(core->file_buffer + core->IP + sizeof(u32));

        *ptr_inst = create_instruction(0, 0, r1, 0, 0);
        *ptr_imm = htobe64(imm);

        movui(core);
        core->IP = 0;

        assert_int_equal(core->U[r1], imm);
    }

    free(file_buffer);
    free(core);
}

// f1 = imm1
static void test_movfi(void **state)
{
    (void)state;
    core_t *core = core_init();

    size_t file_size = sizeof(u32) + sizeof(u64);
    char *file_buffer = (char *)malloc(file_size);

    if (file_buffer == NULL)
    {
        printf("failed to allocate file buffer of size %lu bytes", file_size);
        free(core);
        exit(1);
    }

    core->file_buffer = file_buffer;

    // U[r1] = imm (test quand U[r1] = 0 et U[r1] =! 0 si MAX_ITERATION grand)
    for (int i = 0; i < MAX_ITERATION; ++i)
    {
        u8 r1 = 2;
        f64 imm = 1;

        u32 *ptr_inst = (u32 *)(core->file_buffer);
        f64 *ptr_imm = (f64 *)(core->file_buffer + sizeof(u32));

        *ptr_inst = create_instruction(0, 0, r1, 0, 0);
        *ptr_imm = imm;

        movfi(core);
        core->IP = 0;

        assert_float_equal(core->F[r1], imm, 0.0001);
    }

    free(file_buffer);
    free(core);
}

// u1 = u2 + u3
static void test_addu(void **state)
{
    (void)state;
    core_t *core = core_init();

    size_t file_size = sizeof(u32);
    char *file_buffer = (char *)malloc(file_size);

    if (file_buffer == NULL)
    {
        printf("failed to allocate file buffer of size %lu bytes", file_size);
        free(core);
        exit(1);
    }
    core->file_buffer = file_buffer;

    for (int i = 0; i < MAX_ITERATION; ++i)
    {
        u8 r1 = (u8)(random() % 32);
        u8 r2 = (u8)(random() % 32);
        u8 r3 = (u8)(random() % 32);

        u64 v2 = (u64)random();
        u64 v3 = (u64)random();

        core->U[r2] = v2;
        core->U[r3] = v3;

        u32 *ptr = (u32 *)(core->file_buffer + core->IP);
        *ptr = create_instruction(0, 0, r1, r2, r3);

        addu(core);
        core->IP = 0;

        if (r2 == r3)
        {
            assert_int_equal(core->U[r1], (v3 + v3));
        }
        else
        {
            assert_int_equal(core->U[r1], (v2 + v3));
        }

        // CF[0] = 1 si opération = 0
        if (core->U[r1] == 0)
        {
            assert_true(core->CF[0]);
        }
    }
    free(file_buffer);
    free(core);
}

// u1 = u2 - u3
static void test_subu(void **state)
{
    (void)state;
    core_t *core = core_init();

    size_t file_size = sizeof(u32);
    char *file_buffer = (char *)malloc(file_size);

    if (file_buffer == NULL)
    {
        printf("failed to allocate file buffer of size %lu bytes", file_size);
        free(core);
        exit(1);
    }
    core->file_buffer = file_buffer;

    for (int i = 0; i < MAX_ITERATION; ++i)
    {
        u8 r1 = (u8)(rand() % 10);        // entre 0 et 9
        u8 r2 = (u8)(10 + (rand() % 10)); // entre 10 et 19
        u8 r3 = (u8)(20 + (rand() % 12)); // entre 20 et 31

        u64 v2 = (u64)random();
        u64 v3 = (u64)random();

        core->U[r2] = v2;
        core->U[r3] = v3;

        u32 *ptr = (u32 *)(core->file_buffer + core->IP);
        *ptr = create_instruction(0, 0, r1, r2, r3);

        core->CF[1] = false;
        core->CF[0] = false;

        subu(core);
        core->IP = 0;

        if (v2 < v3)
        {
            assert_true(core->CF[1]);
        }

        // CF[0] = 1 si opération = 0
        if (core->U[r1] == 0)
        {
            assert_true(core->CF[0]);
        }

        if (r2 == r3)
        {
            assert_int_equal(core->U[r1], (v3 - v3));
        }
        else
        {
            assert_int_equal(core->U[r1], (v2 - v3));
        }


    }
    free(file_buffer);
    free(core);
}

// u1 = u2 * u3
static void test_mulu(void **state)
{
    (void)state;
    core_t *core = core_init();

    size_t file_size = sizeof(u32);
    char *file_buffer = (char *)malloc(file_size);

    if (file_buffer == NULL)
    {
        printf("failed to allocate file buffer of size %lu bytes", file_size);
        free(core);
        exit(1);
    }
    core->file_buffer = file_buffer;

    for (int i = 0; i < MAX_ITERATION; ++i)
    {
        u8 r1 = (u8)(random() % 32);
        u8 r2 = (u8)(random() % 32);
        u8 r3 = (u8)(random() % 32);

        u64 v2 = (u64)random();
        u64 v3 = (u64)random();

        core->U[r2] = v2;
        core->U[r3] = v3;

        u32 *ptr = (u32 *)(core->file_buffer + core->IP);
        *ptr = create_instruction(0, 0, r1, r2, r3);

        mulu(core);
        core->IP = 0;

        if (r2 == r3)
        {
            assert_int_equal(core->U[r1], (v3 * v3));
        }
        else
        {
            assert_int_equal(core->U[r1], (v2 * v3));
        }

        // CF[0] = 1 si opération = 0
        if (core->U[r1] == 0)
        {
            assert_true(core->CF[0]);
        }
    }
    free(file_buffer);
    free(core);
}

// u1 = u2 / u3
static void test_divu(void **state)
{
    (void)state;
    core_t *core = core_init();

    size_t file_size = sizeof(u32);
    char *file_buffer = (char *)malloc(file_size);

    if (file_buffer == NULL)
    {
        printf("failed to allocate file buffer of size %lu bytes", file_size);
        free(core);
        exit(1);
    }
    core->file_buffer = file_buffer;

    i64 rd = 1000;

    for (int i = 0; i < MAX_ITERATION; ++i)
    {
        u8 r1 = (u8)(random() % 32);
        u8 r2 = (u8)(random() % 15);      // 0 à 14
        u8 r3 = (u8)(15 + random() % 17); // 15 à 31

        u64 v2 = (u64)random();
        u64 v3 = (u64)(random() % rd);

        core->U[r2] = v2;
        core->U[r3] = v3;

        u32 *ptr = (u32 *)(core->file_buffer);
        *ptr = create_instruction(0, 0, r1, r2, r3);

        core->CF[0] = false;
        core->CF[1] = false;

        divu(core);
        core->IP = 0;

        if (v3 == 0)
        {
            // TODO : pour l'instant
            assert_int_equal(core->U[r1], 0);
            assert_true(core->CF[1]);
        }
        else if (v3 != 0)
        {
            assert_int_equal(core->U[r1], (v2 / v3));
        }

        // CF[0] = 1 si opération = 0
        if (core->U[r1] == 0)
        {
            assert_true(core->CF[0]);
        }
    }

    free(file_buffer);
    free(core);
}

// u1 = u2 % u3
static void test_modu(void **state)
{
    (void)state;
    core_t *core = core_init();

    size_t file_size = sizeof(u32);
    char *file_buffer = (char *)malloc(file_size);

    if (file_buffer == NULL)
    {
        printf("failed to allocate file buffer of size %lu bytes", file_size);
        free(core);
        exit(1);
    }
    core->file_buffer = file_buffer;

    i64 rd = 1000;

    for (int i = 0; i < MAX_ITERATION; ++i)
    {
        u8 r1 = (u8)(random() % 32);
        u8 r2 = (u8)(random() % 15);      // 0 à 14
        u8 r3 = (u8)(15 + random() % 17); // 15 à 31

        u64 v2 = (u64)random();
        u64 v3 = (u64)(random() % rd);

        core->U[r2] = v2;
        core->U[r3] = v3;

        u32 *ptr = (u32 *)(core->file_buffer);
        *ptr = create_instruction(0, 0, r1, r2, r3);

        core->CF[0] = false;
        core->CF[1] = false;

        modu(core);
        core->IP = 0;

        if (v3 == 0)
        {
            // TODO : pour l'instant
            assert_int_equal(core->U[r1], 0);
            assert_true(core->CF[1]);
        }
        else if (v3 != 0)
        {
            assert_int_equal(core->U[r1], (v2 % v3));
        }

        // CF[0] = 1 si opération = 0
        if (core->U[r1] == 0)
        {
            assert_true(core->CF[0]);
        }
    }

    free(file_buffer);
    free(core);
}

// u1 += u2 * u3
static void test_fmau(void **state)
{
    (void)state;
    core_t *core = core_init();

    size_t file_size = sizeof(u32);
    char *file_buffer = (char *)malloc(file_size);

    if (file_buffer == NULL)
    {
        printf("failed to allocate file buffer of size %lu bytes", file_size);
        free(core);
        exit(1);
    }
    core->file_buffer = file_buffer;

    for (int i = 0; i < MAX_ITERATION; ++i)
    {
        u8 r1 = (u8)(random() % 10);
        u8 r2 = (u8)(10 + random() % 10);
        u8 r3 = (u8)(20 + random() % 12);

        u64 v1 = (u64)random();
        u64 v2 = (u64)random();
        u64 v3 = (u64)random();

        core->U[r1] = v1;
        core->U[r2] = v2;
        core->U[r3] = v3;

        u32 *ptr = (u32 *)(core->file_buffer + core->IP);
        *ptr = create_instruction(0, 0, r1, r2, r3);

        fmau(core);
        core->IP = 0;

        assert_int_equal(core->U[r1], v1 + v2 * v3);

        // CF[0] = 1 si opération = 0
        if (core->U[r1] == 0)
        {
            assert_true(core->CF[0]);
        }
    }

    free(file_buffer);
    free(core);
}

// u1 = sqrt(u2)
static void test_sqrtu(void **state)
{
    (void)state;
    core_t *core = core_init();

    size_t file_size = sizeof(u32);
    char *file_buffer = (char *)malloc(file_size);

    if (file_buffer == NULL)
    {
        printf("failed to allocate file buffer of size %lu bytes", file_size);
        free(core);
        exit(1);
    }
    core->file_buffer = file_buffer;

    for (int i = 0; i < MAX_ITERATION; ++i)
    {
        u8 r1 = (u8)(random() % 32);
        u8 r2 = (u8)(random() % 32);

        u64 v2 = (u64)random();
        core->U[r2] = v2;

        u32 *ptr = (u32 *)(core->file_buffer);
        *ptr = create_instruction(0, 0, r1, r2, 0);

        sqrtu(core);
        core->IP = 0;

        assert_int_equal(core->U[r1], (u64)sqrt((f64)v2));

        // CF[0] = 1 si opération = 0
        if (core->U[r1] == 0)
        {
            assert_true(core->CF[0]);
        }
    }

    free(file_buffer);
    free(core);
}

// u1 = log(u2)
static void test_logu(void **state)
{
    (void)state;
    core_t *core = core_init();

    size_t file_size = sizeof(u32);
    char *file_buffer = (char *)malloc(file_size);

    if (file_buffer == NULL)
    {
        printf("failed to allocate file buffer of size %lu bytes", file_size);
        free(core);
        exit(1);
    }
    core->file_buffer = file_buffer;

    // manque le cas où random = 0;

    for (int i = 0; i < MAX_ITERATION; ++i)
    {
        u8 r1 = (u8)(random() % 32);
        u8 r2 = (u8)(random() % 32);

        u64 v2 = (u64)random();
        core->U[r2] = v2;

        u32 *ptr = (u32 *)(core->file_buffer);
        *ptr = create_instruction(0, 0, r1, r2, 0);

        logu(core);
        core->IP = 0;

        if (v2 != 0)
        {
            assert_int_equal(core->U[r1], (u64)log10((f64)v2));
        }

        // CF[0] = 1 si opération = 0
        if (core->U[r1] == 0)
        {
            assert_true(core->CF[0]);
        }
    }

    free(file_buffer);
    free(core);
}

// u1++
static void test_incu(void **state)
{
    (void)state;
    core_t *core = core_init();

    size_t file_size = sizeof(u32);
    char *file_buffer = (char *)malloc(file_size);

    if (file_buffer == NULL)
    {
        printf("failed to allocate file buffer of size %lu bytes", file_size);
        free(core);
        exit(1);
    }
    core->file_buffer = file_buffer;

    for (int i = 0; i < MAX_ITERATION; ++i)
    {
        u8 r1 = (u8)(random() % 32);
        u64 v1 = (u64)random();

        core->U[r1] = v1;

        u32 *ptr = (u32 *)(core->file_buffer + core->IP);
        *ptr = create_instruction(0, 0, r1, 0, 0);

        incu(core);
        core->IP = 0;

        assert_int_equal(core->U[r1], v1 + 1);
    }

    free(file_buffer);
    free(core);
}

// u1--
static void test_decu(void **state)
{
    (void)state;
    core_t *core = core_init();

    size_t file_size = sizeof(u32);
    char *file_buffer = (char *)malloc(file_size);

    if (file_buffer == NULL)
    {
        printf("failed to allocate file buffer of size %lu bytes", file_size);
        free(core);
        exit(1);
    }
    core->file_buffer = file_buffer;

    for (int i = 0; i < MAX_ITERATION; ++i)
    {
        u8 r1 = (u8)(random() % 32);
        u64 v1 = (u64)random();

        core->U[r1] = v1;

        u32 *ptr = (u32 *)(core->file_buffer + core->IP);
        *ptr = create_instruction(0, 0, r1, 0, 0);

        decu(core);
        core->IP = 0;
        if (core->U[r1] == 0)
        {
            assert_true(core->CF[0]);
        }
        assert_int_equal(core->U[r1], v1 - 1);
    }

    free(file_buffer);
    free(core);
}

// u1 = u2 & u3
static void test_andu(void **state)
{
    (void)state;
    core_t *core = core_init();

    size_t file_size = sizeof(u32);
    char *file_buffer = (char *)malloc(file_size);

    if (file_buffer == NULL)
    {
        printf("failed to allocate file buffer of size %lu bytes", file_size);
        free(core);
        exit(1);
    }
    core->file_buffer = file_buffer;

    for (int i = 0; i < MAX_ITERATION; ++i)
    {
        u8 r1 = (u8)(random() % 32);
        u8 r2 = (u8)(random() % 15);
        u8 r3 = (u8)(15 + random() % 17);

        u64 v2 = (u64)random();
        u64 v3 = (u64)random();

        core->U[r2] = v2;
        core->U[r3] = v3;

        u32 *ptr = (u32 *)(core->file_buffer);
        *ptr = create_instruction(0, 0, r1, r2, r3);

        u64 expected_result = v2 & v3;

        andu(core);
        core->IP = 0;

        if (core->U[r1] == 0)
        {
            assert_true(core->CF[0]);
        }

        assert_int_equal(core->U[r1], expected_result);
    }

    free(file_buffer);
    free(core);
}

// comparaison unisgned int
static void test_cmpu(void **state)
{
    (void)state;
    core_t *core = core_init();

    size_t file_size = sizeof(u32);
    char *file_buffer = (char *)malloc(file_size);

    if (file_buffer == NULL)
    {
        printf("failed to allocate file buffer of size %lu bytes", file_size);
        free(core);
        exit(1);
    }
    core->file_buffer = file_buffer;

    // U[r1] == U[r2]
    for (int i = 0; i < MAX_ITERATION; ++i)
    {

        u8 r1 = (u8)(rand() % 32);
        u8 r2 = (u8)(rand() % 32);

        u64 v1 = (u64)rand();

        core->U[r1] = v1;
        core->U[r2] = v1;

        u32 *ptr = (u32 *)(core->file_buffer + core->IP);
        *ptr = create_instruction(0, 0, r1, r2, 0);

        cmpu(core);
        core->IP = 0;

        assert_true(core->CF[7]);
        assert_false(core->CF[6]);
        assert_false(core->CF[5]);
        assert_true(core->CF[4]);
        assert_true(core->CF[2]);
        assert_false(core->CF[1]);
        assert_false(core->CF[0]);
    }

    // U[r1] > U[r2]
    for (int i = 0; i < MAX_ITERATION; ++i)
    {
        u8 r1 = (u8)(rand() % 15);
        u8 r2 = (u8)(15 + (rand() % 17));

        u64 v1 = (u64)((rand() % (RAND_MAX / 2)) + (RAND_MAX / 2));
        u64 v2 = (u64)(rand() % (RAND_MAX / 2));

        core->U[r1] = v1;
        core->U[r2] = v2;

        u32 *ptr = (u32 *)(core->file_buffer);
        *ptr = create_instruction(0, 0, r1, r2, 0);

        cmpu(core);
        core->IP = 0;

        assert_false(core->CF[7]);
        assert_true(core->CF[6]);
        assert_true(core->CF[5]);
        assert_true(core->CF[4]);
        assert_false(core->CF[2]);
        assert_false(core->CF[1]);
        assert_false(core->CF[0]);
    }

    free(file_buffer);
    free(core);
}

// jump if lower
static void test_jl(void **state)
{
    (void)state;
    core_t *core = core_init();

    size_t rd = 100;
    size_t file_size = (sizeof(u32) + sizeof(u64)) * rd;
    char *file_buffer = (char *)malloc(file_size);

    if (file_buffer == NULL)
    {
        printf("failed to allocate file buffer of size %lu bytes", file_size);
        free(core);
        exit(1);
    }
    core->file_buffer = file_buffer;

    // dépassement ?
    // A GERER ==> DONNE PROBLEME MALLOC DEVRAIT GERER LE FAIT DE DEPASSER, MESSAGE DERREUR??

    // Test sans dépassement
    // Doit jumper
    core->CF[3] = true;
    for (int i = 0; i < MAX_ITERATION; ++i)
    {
        core->IP = 0;

        // TODO : vérifier que ça dépasse vraiment pas la taille du buffer
        u8 size = (u8)(rand() % (int)file_size);

        u64 *ptr_addr = (u64 *)(core->file_buffer + core->IP + sizeof(u32));
        *ptr_addr = htobe64(size);

        jl(core);

        assert_int_equal(core->IP, size);
    }

    // Ne doit pas jumper
    core->CF[3] = false;
    for (int i = 0; i < MAX_ITERATION; ++i)
    {
        core->IP = 0;
        // TODO : vérifier que ça dépasse vraiment pas la taille du buffer
        u8 size = (u8)(rand() % (int)file_size);
        u64 *ptr_addr = (u64 *)(core->file_buffer + core->IP + sizeof(u32));
        *ptr_addr = htobe64(size);

        jl(core);

        assert_int_equal(core->IP, 12);
    }

    free(file_buffer);
    free(core);
}

//
/*static void test_outu(void **state)
{
    core_t* core = core_init();
    u8 * chaine = "hello";
    core->U[0] = (u64)(*chaine);

    u64 *ptr = (u64 *)(core->file_buffer + core->IP);
    *ptr = 0;

    int out = open("./out.dat", O_CREAT | O_WRONLY | O_TRUNC, 0600); // peut creer des problemes
    dup2(out, STDOUT_FILENO); // aussi
    outu(core);
    dup2(STDOUT_FILENO, out);

    close(out);

    core_drop(core);
} */

//
int main(void)
{
    int result = 0;
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_loadu),
        cmocka_unit_test(test_loadf),
        cmocka_unit_test(test_storeu),
        cmocka_unit_test(test_storef),
        cmocka_unit_test(test_movu),
        cmocka_unit_test(test_movf),
        cmocka_unit_test(test_movui),
        cmocka_unit_test(test_movfi),
        cmocka_unit_test(test_addu),
        cmocka_unit_test(test_subu),
        cmocka_unit_test(test_mulu),
        cmocka_unit_test(test_divu),
        cmocka_unit_test(test_modu),
        cmocka_unit_test(test_fmau),
        cmocka_unit_test(test_sqrtu),
        cmocka_unit_test(test_logu),
        cmocka_unit_test(test_incu),
        cmocka_unit_test(test_decu),
        cmocka_unit_test(test_andu),
        cmocka_unit_test(test_cmpu),
        cmocka_unit_test(test_jl),
        // cmocka_unit_test(test_outu),
    };
    result |= cmocka_run_group_tests_name("vm", tests, NULL, NULL);

    return result;
}
