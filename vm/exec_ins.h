#include "registre.h"
#include "opcode.h"
#include <stdio.h>
#include <math.h>

#ifndef EXECUTE_INSTRUCTION_H
#define EXECUTE_INSTRUCTION_H

void execute_instruction(CPU *cpu, enum Opcode opcode, u16 unused, u8 dst, u8 src1, u8 src2)
{
    switch (opcode)
    {
    // Unsigned arithmetic and logic instructions
    case ADDU:
        cpu->scalar_regs.U[dst] = cpu->scalar_regs.U[src1] + cpu->scalar_regs.U[src2];
        break;
    case SUBU:
        cpu->scalar_regs.U[dst] = cpu->scalar_regs.U[src1] - cpu->scalar_regs.U[src2];
        break;
    case MULU:
        cpu->scalar_regs.U[dst] = cpu->scalar_regs.U[src1] * cpu->scalar_regs.U[src2];
        break;
    case DIVU:
        cpu->scalar_regs.U[dst] = cpu->scalar_regs.U[src1] / cpu->scalar_regs.U[src2];
        break;
    case MODU:
        cpu->scalar_regs.U[dst] = cpu->scalar_regs.U[src1] % cpu->scalar_regs.U[src2];
        break;
    case FMAU:
        cpu->scalar_regs.U[dst] = cpu->scalar_regs.U[dst] + cpu->scalar_regs.U[src1] * cpu->scalar_regs.U[src2];
        break;
    case SQRTU:
        cpu->scalar_regs.U[dst] = sqrt(cpu->scalar_regs.U[src1]);
        break;
    case LOGU:
        cpu->scalar_regs.U[dst] = log(cpu->scalar_regs.U[src1]);
        break;
    case INCU:
        cpu->scalar_regs.U[dst]++;
        break;
    case DECU:
        cpu->scalar_regs.U[dst]--;
        break;
    default:
        printf("Error: Unimplemented instruction\n");
        printf("unused%d", unused);
    }
}

#endif