#include <math.h>
#include <time.h>

#include <stdio.h>
#include <stdlib.h>

#include "vm.h"
#include "instruction.h"

#define MAX_SAMPLES 5
#define ITERATION 50

typedef struct opcode_s
{
    void (*opcode)(core_t *);
    u8 *name;
} opcode_t;

//
static core_t *core_init()
{
    core_t *core = (core_t *)malloc(sizeof(core_t));
    if (core == NULL)
    {
        printf("failed to allocate core of size %zu bytes", sizeof(core_t));
        exit(1);
    }
    u8 *memory = (u8 *)malloc(MAX_MEMORY_SIZE);
    if (memory == NULL)
    {
        printf("failed to allocate memory of size %u bytes", MAX_MEMORY_SIZE);
        free(core);
        exit(1);
    }

    u64 *ptr = (u64 *)memory;
    for (int i = 0; i < MAX_MEMORY_SIZE / 8; ++i)
    {
        // attention, peut casser à tout moment
        *(ptr + i) = (u64)(i % (MAX_MEMORY_SIZE / 4)); // pour les déplacements mémoires
    }

    core->memory = memory;

    // LES INTIALISER AVEC DES PLUS GRANDES VALEURS ???

    for (int i = 0; i < NUMBER_SCALAR_REGISTER; ++i)
    {
        core->U[i] = (rand() % (MAX_MEMORY_SIZE / 3)); // pour les déplacements mémoires
        core->S[i] = (i64)rand();
        core->F[i] = (f64)rand();
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
    return htobe32((opcode & 0xFF) << (SIZE_INSTRUCTION - 8) | (offset & 0x1FF) << (SIZE_INSTRUCTION - 17) | (register_1 & 0x1F) << (SIZE_INSTRUCTION - 22) | (register_2 & 0x1F) << (SIZE_INSTRUCTION - 27) | register_3 & 0x1F);
}

//
void sort_f64(f64 *restrict a, u64 n)
{
    for (u64 i = 0; i < n; i++)
        for (u64 j = i + 1; j < n; j++)
            if (a[i] > a[j])
            {
                f64 tmp = a[i];

                a[i] = a[j];
                a[j] = tmp;
            }
}

//
f64 mean_f64(f64 *restrict a, u64 n)
{
    f64 m = 0.0;

    for (u64 i = 0; i < n; i++)
        m += a[i];

    m /= (f64)n;

    return m;
}

//
f64 stddev_f64(f64 *restrict a, u64 n)
{
    f64 d = 0.0;
    f64 m = mean_f64(a, n);

    for (u64 i = 0; i < n; i++)
        d += (a[i] - m) * (a[i] - m);

    d /= (f64)(n - 1);

    return sqrt(d);
}

//
void mesure_performance_u64_scalaire(void (*opcode)(core_t *), u64 r, const u8 *title)
{
    core_t *core = core_init();

    size_t size_file_buffer = (sizeof(u32) + sizeof(u64)) * r; // bon peut-etre un peu bete (marche que scalaire, c'est pour les jump)
    // mais allocation mémoire assez grande
    u8 *file_buffer = (u8 *)malloc(size_file_buffer);

    if (file_buffer == NULL)
    {
        printf("failed to allocate file buffer of size %lu bytes", size_file_buffer);
        core_drop(core);
        exit(1);
    }

    core->file_buffer = file_buffer;

    f64 elapsed = 0.0;
    struct timespec t1, t2;
    f64 samples[r];

    for (int i = 0; i < r; ++i)
    {
        u64 to_read = core->IP;
        // initialisation de l'instruction
        u8 r1 = rand() % 32;
        u8 r2 = rand() % 32;
        u8 r3 = rand() % 32;
        u16 offset = 0;

        u32 *ptr_inst = (u32 *)(core->file_buffer + core->IP);
        u64 *ptr_imm = (u64 *)(core->file_buffer + core->IP + sizeof(u32));

        *ptr_inst = create_instruction(0, offset, r1, r2, r3);
        *ptr_imm = htobe64((rand() % size_file_buffer)); // pour les sauts d'adresse mémoire

        core->U[r1] = 1 + (rand() % (RAND_MAX - 1));
        core->U[r2] = 1 + (rand() % (RAND_MAX - 1));
        core->U[r3] = 1 + (rand() % (RAND_MAX - 1));

        clock_gettime(CLOCK_MONOTONIC_RAW, &t1);
        for (int j = 0; j < ITERATION; ++j)
        {
            opcode(core);
            core->IP = to_read;
        }
        clock_gettime(CLOCK_MONOTONIC_RAW, &t2);

        elapsed = (f64)(t2.tv_nsec - t1.tv_nsec) / (f64)ITERATION;
        samples[i] = elapsed;
    }

    core_drop(core);

    sort_f64(samples, r);
    f64 min = samples[0];
    f64 max = samples[r - 1];
    f64 mean = mean_f64(samples, r);
    f64 dev = stddev_f64(samples, r);
    f64 opns = (f64)(r * ITERATION) / mean;

    printf("%10s; %10lu; %15.3lf; %15.3lf; %15.2lf; %15.3lf (%6.1lf %%); %16.2lf;\n",
           title,
           r * ITERATION,
           min,
           mean,
           max,
           dev,
           (dev * 100.0 / mean),
           opns);
}

//
void mesure_performance_s64_scalaire(void (*opcode)(core_t *), u64 r, const u8 *title)
{
    core_t *core = core_init();

    size_t size_file_buffer = (sizeof(u32) + sizeof(u64)) * r; // bon peut-etre un peu bete (marche que scalaire, c'est pour les jump)
    // mais allocation mémoire assez grande
    u8 *file_buffer = (u8 *)malloc(size_file_buffer);

    if (file_buffer == NULL)
    {
        printf("failed to allocate file buffer of size %lu bytes", size_file_buffer);
        core_drop(core);
        exit(1);
    }

    core->file_buffer = file_buffer;

    f64 elapsed = 0.0;
    struct timespec t1, t2;
    f64 samples[r];

    for (int i = 0; i < r; ++i)
    {
        u64 to_read = core->IP;
        // initialisation de l'instruction
        u8 r1 = rand() % 32;
        u8 r2 = rand() % 32;
        u8 r3 = rand() % 32;
        u16 offset = 0;

        u32 *ptr_inst = (u32 *)(core->file_buffer + core->IP);
        u64 *ptr_imm = (u64 *)(core->file_buffer + core->IP + sizeof(u32));

        *ptr_inst = create_instruction(0, offset, r1, r2, r3);
        *ptr_imm = htobe64((rand() % size_file_buffer)); // pour les sauts d'adresse mémoire

        core->S[r1] = (i64)(1 + (rand() % (RAND_MAX - 1)));
        core->S[r2] = (i64)(1 + (rand() % (RAND_MAX - 1)));
        core->S[r3] = (i64)(1 + (rand() % (RAND_MAX - 1)));

        clock_gettime(CLOCK_MONOTONIC_RAW, &t1);
        for (int j = 0; j < ITERATION; ++j)
        {
            opcode(core);
            core->IP = to_read;
        }
        clock_gettime(CLOCK_MONOTONIC_RAW, &t2);

        elapsed = (f64)(t2.tv_nsec - t1.tv_nsec) / (f64)ITERATION;
        samples[i] = elapsed;
    }

    core_drop(core);

    sort_f64(samples, r);
    f64 min = samples[0];
    f64 max = samples[r - 1];
    f64 mean = mean_f64(samples, r);
    f64 dev = stddev_f64(samples, r);
    f64 opns = (f64)(r * ITERATION) / mean;

    printf("%10s; %10lu; %15.3lf; %15.3lf; %15.2lf; %15.3lf (%6.1lf %%); %16.2lf;\n",
           title,
           r * ITERATION,
           min,
           mean,
           max,
           dev,
           (dev * 100.0 / mean),
           opns);
}

//
int main()
{
    opcode_t opcode_u64_tobench[] =
        {
            //{loadu, "loadu"},
            //{storeu, "storeu"},
            {movu, "movu"},
            {movui, "movui"},
            {addu, "addu"},
            {subu, "subu"},
            {mulu, "mulu"},
            {divu, "divu"},
            {modu, "modu"},
            {fmau, "fmau"},
            {sqrtu, "sqrtu"},
            //{logu, "logu"},
            {incu, "incu"},
            {decu, "decu"},
            {andu, "andu"},
            {oru, "oru"},
            {xoru, "xoru"},
            {cmpu, "cmpu"},
            {jne, "jne"},
            {jge, "jge"},
            {jl, "jle"},
            {jle, "jl"},
        };
    
    opcode_t opcode_s64_tobench[] =
    {
        {adds, "adds"},
        {subs, "subs"},
        {muls, "muls"},
        {divs, "divs"},
        {mods, "mods"},
        {fmas, "fmas"},
        {sqrts, "sqrts"},
        //{logs, "logs"},
        {ands, "ands"},
        {ors, "ors"},
        {xors, "xors"},
    };

    // print header
    printf("%10s; %10s; %15s; %15s; %15s; %25s; %18s;\n",
           "opcode",
           "r", "min (ns)", "mean (ns)", "max (ns)", "stddev (%)", "opération/ns");

    u64 r = 1000;

    for (int i = 0; i < 19; ++i)
    {
        mesure_performance_u64_scalaire(opcode_u64_tobench[i].opcode, r, opcode_u64_tobench[i].name);
    }

        for (int i = 0; i < 10; ++i)
    {
        mesure_performance_s64_scalaire(opcode_s64_tobench[i].opcode, r, opcode_s64_tobench[i].name);
    }

    return 0;
}