#include <stddef.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#ifndef VM_H
#define VM_H
#include "utils.h"

#define NUMBER_SCALAR_REGISTER 32
#define NUMBER_VECTOR_REGISTER 32
#define NUMBER_SCALAR_IN_VECTOR_REGISTER 8
#define SIZE_INSTRUCTION 32
// SIZE_INSTRUCTION_IN_BYTE = 4
#define SIZE_INSTRUCTION_IN_BYTE SIZE_INSTRUCTION / 8
#define SIZE_IMMEDIATE_IN_BYTE 8
#define MAX_INSTRUCTION_NUMBER 256
#define MAX_MEMORY_SIZE 1024 * 1024
#define MAX_FILE_BUFFER_SIZE 256
// IF YOU CHANGE MAX_FILE_NAME_SIZE,
// YOU NEED TO CHANGE ARGUMENTS OF FSCANF IN READ_CONFIG FUNCTION,
// OTHERWISE, THIS DOESN'T WORK PROPERLY
#define MAX_FILE_NAME_SIZE 256
#define MAX_MUTEX 16

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;
typedef int64_t i64;
typedef double f64;

typedef struct instruction_s
{
    u8 opcode;
    u16 offset;
    u8 register_1;
    u8 register_2;
    u8 register_3;

} instruction_t;

instruction_t instruction_new(u32 instruction);

typedef struct header_s
{
    u64 magic_number;
    u64 header_size;
    u64 address_data;        // Data section address
    u64 size_data;           // Data section size
    u64 address_code;        // Code section address
    u64 size_code;           // Code section size
    u64 address_parallel_on; // Parallel on section address
    u64 size_parallel_on;
    u64 core_number;
    u64 size_total; // Size total
} header_t;

typedef struct core_s
{
    // scalar register
    u64 U[NUMBER_SCALAR_REGISTER];
    i64 S[NUMBER_SCALAR_REGISTER];
    f64 F[NUMBER_SCALAR_REGISTER];

    // vector register
    u64 V[NUMBER_VECTOR_REGISTER][NUMBER_SCALAR_IN_VECTOR_REGISTER];
    i64 T[NUMBER_VECTOR_REGISTER][NUMBER_SCALAR_IN_VECTOR_REGISTER];
    f64 G[NUMBER_VECTOR_REGISTER][NUMBER_SCALAR_IN_VECTOR_REGISTER];

    // comparaison flag register
    bool CF[8];

    // Instruction pointer
    u64 IP;

    u8 *memory;
    char *file_buffer;
    header_t header;
    // FILE *fd;

    // TODO Check affinity
    u32 id;
    u32 given_id;
    u32 offset;
    bool type;              // coretype, 0 = compute, 1 = management
    pthread_mutex_t *mutex; // mutex
    // pthread_mutex_t mutex_tab[MAX_MUTEX_PER_CORE];

    // Pointer to the execution function
    // void (*insn_exec)(...);
} core_t;

typedef struct to_send_registers_s
{
    // scalar register
    u64 U[NUMBER_VECTOR_REGISTER];
    i64 S[NUMBER_VECTOR_REGISTER];
    f64 F[NUMBER_VECTOR_REGISTER];

    // vector register
    u64 V[NUMBER_VECTOR_REGISTER][NUMBER_SCALAR_IN_VECTOR_REGISTER];
    i64 T[NUMBER_VECTOR_REGISTER][NUMBER_SCALAR_IN_VECTOR_REGISTER];
    f64 G[NUMBER_VECTOR_REGISTER][NUMBER_SCALAR_IN_VECTOR_REGISTER];

    // comparaison flag register
    bool CF[8];
} to_send_registers_t;

#pragma pack(1)
typedef struct program_thread_data_s
{
    pthread_t tid;
    u16 index;
    char *file_buffer;
    u32 core_id;
    u32 given_id;
    u32 core_offset;
    u32 numa_id;
    u32 memory_size;
    u8 *memory_address;
    bool management;
    u64 IP;
    pthread_mutex_t *mutex;

    to_send_registers_t *registers;

} program_thread_data_t;

core_t *core_new(program_thread_data_t *args);

u64 get_immediate(core_t *core);

void core_drop(core_t *self);

void *execute_program_thread(void *args);
void read_config(char *config_file_name, char **file_buffer_list, u16 *number_of_file);
void set_up_instruction_set();

#endif
