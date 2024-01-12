#include "tools.h"
#include "cpu.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//
void sort_f64(double *restrict a, unsigned long long n)
{
    for (unsigned long long i = 0; i < n; i++)
        for (unsigned long long j = i + 1; j < n; j++)
            if (a[i] > a[j])
            {
                double tmp = a[i];

                a[i] = a[j];
                a[j] = tmp;
            }
}

//
double mean_f64(double *restrict a, unsigned long long n)
{
    double m = 0.0;

    for (unsigned long long i = 0; i < n; i++)
        m += a[i];

    m /= (double)n;

    return m;
}

//
double stddev_f64(double *restrict a, unsigned long long n)
{
    double d = 0.0;
    double m = mean_f64(a, n);

    for (unsigned long long i = 0; i < n; i++)
        d += (a[i] - m) * (a[i] - m);

    d /= (double)(n - 1);

    return sqrt(d);
}

Instruction parse_instruction(u32 instruction)
{
    Instruction inst;
    inst.opcode = instruction >> (SIZE_INSTRUCTION - 8) & 0xFF;
    inst.unused = instruction >> (SIZE_INSTRUCTION - 17) & 0x1FF;
    inst.destination = instruction >> (SIZE_INSTRUCTION - 22) & 0x1F;
    inst.source_1 = instruction >> (SIZE_INSTRUCTION - 27) & 0x1F;
    inst.source_2 = instruction & 0x1F;
    return inst;
}

u8 get_offset(u16 unused)
{
    // les 8 bits de poids faible
    return unused & 0xFF;
}

int get_immediate(CPU *cpu)
{
    int i = 0;
    int immediat = 0;
    while (i < 8)
    {
        char *value_adress = &(cpu->BinaryFile[cpu->IP]);
        int value = (int)*value_adress;
        immediat += value << (8 - 1);
        i++;
        cpu->IP += 1;
    }
    return immediat;
}
