#include <stdlib.h>
#include <string.h>
#include "../vm.h"
#include "../instruction.h"

void (*instruction_set[MAX_INSTRUCTION_NUMBER])(core_t *);

void set_up_instruction_set()
{
    for (u8 index = 0; index < MAX_INSTRUCTION_NUMBER - 1; index++)
        instruction_set[index] = undefined_instruction;
    if (MAX_INSTRUCTION_NUMBER != 256)
        warn("Consider changing the %s index type to another type to avoid potential issues.", "u8");
    instruction_set[0] = loadu;
    // instruction_set[1] = loads;
    // instruction_set[2] = loadf;
    // instruction_set[3] = loadv;
    // instruction_set[4] = loadt;
    // instruction_set[5] = loadg;
    instruction_set[6] = storeu;
    // instruction_set[7] = stores;
    // instruction_set[8] = storef;
    // instruction_set[9] = storev;
    // instruction_set[10] = storet;
    // instruction_set[11] = storeg;
    // instruction_set[12] = mov;
    instruction_set[13] = movu;
    // instruction_set[14] = movs;
    // instruction_set[15] = movf;
    // instruction_set[16] = movv;
    // instruction_set[17] = movt;
    // instruction_set[18] = movg;
    instruction_set[19] = movui;
    // instruction_set[20] = movsi;
    // instruction_set[21] = movfi;
    // instruction_set[22] = movvi;
    // instruction_set[23] = movti;
    // instruction_set[24] = movgi;
    // instruction_set[25] = cvtus;
    // instruction_set[26] = cvtsu;
    // instruction_set[27] = cvtuf;
    // instruction_set[28] = cvtfu;
    // instruction_set[29] = cvtsf;
    // instruction_set[30] = cvtfs;
    instruction_set[31] = addu;
    instruction_set[32] = subu;
    instruction_set[33] = mulu;
    instruction_set[34] = divu;
    // instruction_set[35] = modu;
    instruction_set[36] = fmau;
    // instruction_set[37] = sqrtu;
    // instruction_set[38] = logu;
    instruction_set[39] = incu;
    // instruction_set[40] = decu;
    // instruction_set[41] = andu;
    // instruction_set[42] = oru;
    // instruction_set[43] = xoru;
    // instruction_set[44] = shlu;
    // instruction_set[45] = shru;
    // instruction_set[46] = rolu;
    // instruction_set[47] = roru;
    // instruction_set[48] = popcntu;
    // instruction_set[49] = lmbu;
    // instruction_set[50] = adds;
    // instruction_set[51] = subs;
    // instruction_set[52] = muls;
    // instruction_set[53] = divs;
    // instruction_set[54] = mods;
    // instruction_set[55] = fmas;
    // instruction_set[56] = sqrts;
    // instruction_set[57] = logs;
    // instruction_set[58] = ands;
    // instruction_set[59] = ors;
    // instruction_set[60] = xors;
    // instruction_set[61] = shls;
    // instruction_set[62] = shrs;
    // instruction_set[63] = rols;
    // instruction_set[64] = rors;
    // instruction_set[65] = popcnts;
    // instruction_set[66] = lmbs;
    // instruction_set[67] = addf;
    // instruction_set[68] = subf;
    // instruction_set[69] = mulf;
    // instruction_set[70] = divf;
    // instruction_set[71] = fmaf_;
    // instruction_set[72] = sqrtf_;
    // instruction_set[73] = logf_;
    instruction_set[74] = cmpu;
    // instruction_set[75] = cmps;
    // instruction_set[76] = cmpf;
    // instruction_set[77] = je;
    // instruction_set[78] = jne;
    // instruction_set[79] = jge;
    instruction_set[80] = jl;
    // instruction_set[81] = jle;
    // instruction_set[82] = jz;
    // instruction_set[83] = jnz;
    instruction_set[84] = outu;
    // instruction_set[85] = outs;
    // instruction_set[86] = outf;
    // instruction_set[87] = outa;
    instruction_set[88] = outb;
    // instruction_set[89] = outx;
    instruction_set[90] = hlt;
}