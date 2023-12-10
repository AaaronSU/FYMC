#ifndef INIT_H
#define INIT_H
#include "type.h"

#define NUMBER_SCALAR_REGISTER 32
#define NUMBER_VECTOR_REGISTER 32
#define SIZE_INSTRUCTION 32
#define SIZE_VECTOR 8

typedef struct {

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

    // instruction pointer register
    u64 IP;

} CPU;


#endif
