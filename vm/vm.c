#define _GNU_SOURCE
#include <sched.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <libgen.h>
#include <errno.h>
#include <pthread.h>
#include "vm.h"

extern void (*instruction_set[MAX_INSTRUCTION_NUMBER])(core_t *);

instruction_t instruction_new(u32 instruction)
{
    instruction = htobe32(instruction);
    return (instruction_t){
        .opcode = (u8)(instruction >> (SIZE_INSTRUCTION - 8) & 0xFF),
        .offset = instruction >> (SIZE_INSTRUCTION - 17) & 0x1FF,
        .register_1 = instruction >> (SIZE_INSTRUCTION - 22) & 0x1F,
        .register_2 = instruction >> (SIZE_INSTRUCTION - 27) & 0x1F,
        .register_3 = instruction & 0x1F,
    };
}

core_t *core_new(program_thread_data_t *args)
{
    core_t *core = (core_t *)malloc(sizeof(core_t)); // Allocate memory for the core
    if (core == NULL)
    {
        error("failed to allocate core of size %zu bytes", sizeof(core_t));
        exit(EXIT_FAILURE);
    }

    char *config_file_line = args->file_buffer;
    struct stat st;

    if (stat(config_file_line, &st) != 0)
    {
        error("File not found or error accessing file %s.\n", config_file_line);
        exit(EXIT_FAILURE);
    }
    FILE *file = fopen(args->file_buffer, "rb");
    uint64_t total_size = (uint64_t)st.st_size;

    char *buffer = malloc(total_size);
    if (buffer == NULL)
    {
        error("File not found or error accessing file %s.\n", config_file_line);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    if (fread(buffer, 1, total_size, file) != total_size)
    {
        fprintf(stderr, "Erreur lors du chargement de données dans le buffer.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    fclose(file);

    core->file_buffer = buffer;

    core->id = args->core_id + args->core_offset;
    core->type = args->management;
    core->offset = args->core_offset;
    core->given_id = args->given_id;

    u8 *memory;

    // If management core
    if (core->type)
    {
        core->mutex = malloc(MAX_MUTEX * sizeof(pthread_mutex_t));
        for (u16 k = 0; k < MAX_MUTEX; ++k)
            pthread_mutex_init(&core->mutex[k], NULL);

        for (int i = 0; i < NUMBER_SCALAR_REGISTER; i++)
        {
            core->U[i] = 0;
            core->S[i] = 0;
            core->F[i] = 0.0;
        }
        for (int i = 0; i < NUMBER_VECTOR_REGISTER; i++)
        {
            for (int j = 0; j < NUMBER_SCALAR_IN_VECTOR_REGISTER; j++)
            {
                core->V[i][j] = 0;
                core->T[i][j] = 0;
                core->G[i][j] = 0.0;
            }
        }
        for (int i = 0; i < 8; i++)
        {
            core->CF[i] = false;
        }

        header_t header = *(header_t *)(core->file_buffer);
        core->IP = htobe64(header.address_code);

        memory = (u8 *)malloc(MAX_MEMORY_SIZE);
        memset(memory, 0, MAX_MEMORY_SIZE);
    }
    else
    {
        // TODO only cpy what's important (cf instruction/par.c)
        for (int i = 0; i < NUMBER_SCALAR_REGISTER; i++)
        {
            core->U[i] = args->registers->U[i];
            core->S[i] = args->registers->S[i];
            core->F[i] = args->registers->F[i];
        }
        for (int i = 0; i < NUMBER_VECTOR_REGISTER; i++)
        {
            for (int j = 0; j < NUMBER_SCALAR_IN_VECTOR_REGISTER; j++)
            {
                core->V[i][j] = args->registers->V[i][j];
                core->T[i][j] = args->registers->T[i][j];
                core->G[i][j] = args->registers->G[i][j];
            }
        }
        for (int i = 0; i < 8; i++)
        {
            core->CF[i] = args->registers->CF[i];
        }

        memcpy(core->CF, args->registers->CF, 8);
        memory = args->memory_address;
        core->IP = args->IP;
        core->mutex = args->mutex;
    }

    if (memory == NULL)
    {
        free(core);
        error("failed to allocate memory of size %zu bytes", sizeof(core_t));
        exit(EXIT_FAILURE);
    }
    core->memory = memory;

    return core;
}

u64 get_immediate(core_t *core)
{
    u64 immediate = htobe64(*(u64 *)&(core->file_buffer[core->IP]));
    core->IP += SIZE_IMMEDIATE_IN_BYTE;
    return immediate;
}

void core_execute(core_t *self)
{
    header_t header = *(header_t *)(self->file_buffer);

    header.header_size = htobe64(header.header_size);
    header.address_data = htobe64(header.address_data);
    header.size_data = htobe64(header.size_data);
    header.address_code = htobe64(header.address_code);
    header.size_code = htobe64(header.size_code);
    header.address_parallel_on = htobe64(header.address_parallel_on);
    header.size_parallel_on = htobe64(header.size_parallel_on);
    header.core_number = htobe64(header.core_number);
    header.size_total = htobe64(header.size_total);

    DEBUG_PRINT(
        "magic number: %lx\n"
        "header size: %lx\n"
        "address data: %lx\n"
        "size data: %lx\n"
        "address code: %lx\n"
        "size code: %lx\n"
        "address parallel on: %lx\n"
        "size parallel on: %lx\n"
        "core number: %lx\n"
        "size total: %lx\n\n",

        header.magic_number,
        header.header_size,
        header.address_data,        // Data section address
        header.size_data,           // Data section size
        header.address_code,        // Code section address
        header.size_code,           // Code section size
        header.address_parallel_on, // Parallel on section address
        header.size_parallel_on,
        header.core_number,
        header.size_total // Size total);
    );

    // self->IP = header.address_code;

    while (self->IP < header.size_total)
    {
        u8 opcode = *(u8 *)&(self->file_buffer[self->IP]);
        instruction_set[opcode](self);
    }
}

// NOTE: file_buffer are freed after VM execution to avoid double free issues
void core_drop(core_t *self)
{
    if (NULL != self)
    {
        if (self->type)
        {
            if (NULL != self->memory)
                free(self->memory);
            if (NULL != self->file_buffer)
                free(self->file_buffer);
            if (self->mutex)
                free(self->mutex);
        }
        free(self);
    }
}

void read_config(char *config_file_name, char **file_buffer_list, u16 *number_of_file)
{
    FILE *config_file;
    char config_file_line[256];
    u16 i = 0;
    i64 number_of_thread = 0;

    config_file = fopen(config_file_name, "r");

    if (config_file == NULL)
    {
        error("The configuration file %s cannot be opened.\n", config_file_name);
        exit(EXIT_FAILURE);
    }

    if (fscanf(config_file, "%255[^\n]\n", config_file_line) != EOF)
    {
        char *endptr;
        errno = 0;
        number_of_thread = strtol(config_file_line, &endptr, 0);
        // Là faut gérer mieux strtol
        if (number_of_thread == 0)
        {
            error("The configuration file start with %ld thread, it need to be the following format : <int> <path1> <path2> ...\n", number_of_thread);
            fclose(config_file);
            exit(EXIT_FAILURE);
        }

        // Important : déclenche l'erreur lorsqu'il ne s'agit pas un nombre
    }

    // Lire chaque config_file_line du config_file
    while ((fscanf(config_file, "%255[^\n]\n", config_file_line) != EOF) && (i < number_of_thread))
    {
        // printf("config_file_line: %s\n", config_file_line); // Afficher la config_file_line lue
        struct stat st;
        file_buffer_list[i] = (char *)malloc(sizeof(char) * 256);
        char *full_path = file_buffer_list[i];
        snprintf(full_path, sizeof(config_file_line), "%s", config_file_line);

        if (stat(full_path, &st) != 0)
        {
            error("File not found or error accessing file %s.\n", config_file_line);
            fclose(config_file);
            exit(EXIT_FAILURE);
        }
        // printf("full path: %s\n", full_path);

        i++;
    }

    // Fermer le config_file
    fclose(config_file);
    *number_of_file = i;
}

void *execute_program_thread(void *args)
{
    program_thread_data_t *td = (program_thread_data_t *)args;
    getcpu(&td->core_id, &td->numa_id);
    core_t *core = core_new(td);
    core_execute(core);
    core_drop(core);
    return args;
}
