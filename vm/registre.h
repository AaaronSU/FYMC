#ifndef REGISTRE_H
#define REGISTRE_H
#include "type.h"

#define NUMBER_SCALAR_REGISTER 32
#define NUMBER_VECTOR_REGISTER 32
#define MEMORY_SIZE 65536
#define SIZE_INSTRUCTION 32
#define SIZE_VECTOR 8

// scalar register
typedef struct
{
    u64 U[NUMBER_SCALAR_REGISTER];
    i64 S[NUMBER_SCALAR_REGISTER];
    f64 F[NUMBER_SCALAR_REGISTER];
} ScalarRegisters;

// vector register
typedef struct
{
    u64 V[NUMBER_VECTOR_REGISTER][SIZE_VECTOR];
    i64 T[NUMBER_VECTOR_REGISTER][SIZE_VECTOR];
    f64 G[NUMBER_VECTOR_REGISTER][SIZE_VECTOR];
} VectorRegisters;

// Define the system registers
typedef struct
{
    // comparaison flag register
    u8 CF;
    // Instruction pointer
    u64 IP;
} SystemRegisters;

// Define the simulator state
typedef struct
{
    ScalarRegisters scalar_regs;
    VectorRegisters vector_regs;
    SystemRegisters sys_regs;
    u64 memory[MEMORY_SIZE];
} CPU;

#endif