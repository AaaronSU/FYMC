#ifndef REGISTRE_H
#define REGISTRE_H

#include <stdint.h> // c'est mieux je crois
#include "type.h"

#define NUMBER_SCALAR_REGISTER 32
#define NUMBER_VECTOR_REGISTER 32
#define MEMORY_SIZE 65536
#define SIZE_INSTRUCTION 32
#define SIZE_VECTOR 8

// Define the simulator state
typedef struct
{
    // comparaison flag register
    u8 CF;

    // Instruction pointer
    u64 IP;

    // scalar register
    u64 U[NUMBER_SCALAR_REGISTER];
    i64 S[NUMBER_SCALAR_REGISTER];
    f64 F[NUMBER_SCALAR_REGISTER];

    // vector register
    u64 V[NUMBER_VECTOR_REGISTER][SIZE_VECTOR];
    i64 T[NUMBER_VECTOR_REGISTER][SIZE_VECTOR];
    f64 G[NUMBER_VECTOR_REGISTER][SIZE_VECTOR];

    // principal memory : memory data 
    u64 Memory[MEMORY_SIZE];

    // code memory
    u64* BinaryFile;
} CPU;


typedef struct {

    u8 opcode : 8;
    u16 unused : 9;
    u8 destination : 5;
    u8 source_1 : 5;
    u8 source_2 : 5;
    
} Instruction;
#endif


// memory.h => memoire principale qui 