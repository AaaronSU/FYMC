#ifndef TOOLS_H
#define TOOLS_H

#include "type.h"
#include "cpu.h"

Instruction parse_instruction(u32 instruction);
u8 get_offset(u16 unused);
int get_immediate(CPU *cpu);
void print_f64(double *restrict a, unsigned long long n);
void init_f64(double *restrict a, unsigned long long n, const ascii m);
void sort_f64(double *restrict a, unsigned long long n);
f64 mean_f64(double *restrict a, unsigned long long n);
f64 stddev_f64(double *restrict a, unsigned long long n);

#endif