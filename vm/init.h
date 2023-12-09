#ifndef INIT_H
#define INIT_H

#include <stdio.h>
#include <stdint.h>

#define NUMBER_SCALAR_REGISTER 32
#define NUMBER_VECTOR_REGISTER 32
#define SIZE_INSTRUCTION 32
#define SIZE_VECTOR 8
// Data types

typedef unsigned long long u64;
typedef unsigned long u32;
typedef unsigned short u16;
typedef unsigned char u8;

typedef long long i64;
typedef double f64;

// scalar register

extern u64 U[NUMBER_SCALAR_REGISTER];
extern i64 S[NUMBER_SCALAR_REGISTER];
extern f64 F[NUMBER_SCALAR_REGISTER];

// vector register

extern u64 V[NUMBER_VECTOR_REGISTER][SIZE_VECTOR];
extern i64 T[NUMBER_VECTOR_REGISTER][SIZE_VECTOR];
extern f64 G[NUMBER_VECTOR_REGISTER][SIZE_VECTOR];

extern u8 CF;
extern u64 IP;

#endif
