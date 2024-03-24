#ifndef CPU_H
#define CPU_H

#include <stdint.h> // c'est mieux je crois
#include "type.h"

#define NUMBER_SCALAR_REGISTER 32
#define NUMBER_VECTOR_REGISTER 32
#define MEMORY_SIZE 65536
#define SIZE_INSTRUCTION 32
#define SIZE_VECTOR 8
#define INSTRUCTION_BYTES 4

typedef struct core_s
{
    u64 ID;
    u8 type; // coretype, 0 = compute, 1 = management

    // scalar register
    u64 U[NUMBER_SCALAR_REGISTER];
    i64 S[NUMBER_SCALAR_REGISTER];
    f64 F[NUMBER_SCALAR_REGISTER];

    // vector register
    u64 V[NUMBER_VECTOR_REGISTER][SIZE_VECTOR];
    i64 T[NUMBER_VECTOR_REGISTER][SIZE_VECTOR];
    f64 G[NUMBER_VECTOR_REGISTER][SIZE_VECTOR];

    // comparaison flag register
    u8 CF;

    // Instruction pointer
    u64 IP;

    u8 *Memory;
    char *BinaryFile;
    // mutex
    // pthread_mutex_t mutex_tab[MAX_MUTEX_PER_CORE];

    // Pointer to the execution function
    // void (*insn_exec)(...);
} CPU;

typedef struct numa_node_s
{
    u64 ID;
    u64 nb_cores;
    CPU *cores;
    CPU *management_core;

} numa_node_t;

// fonction qui initialise, crée un seul coeur, on appelle dans numa_node_create
CPU *core_create(u8 type, void (insn_exec())); 

// nombre de coeurs etc, fonction qui initialise
numa_node_t *numa_node_create();

/**********************************************************************************************/
typedef struct
{
    u8 opcode : 8;
    u16 unused : 9;
    u8 destination : 5;
    u8 source_1 : 5;
    u8 source_2 : 5;

} Instruction;

#endif