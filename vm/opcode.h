#include "type.h"
#include "tools.h"
#include "cpu.h"
#include "load.h"
#include "conversion.h"
#include "store.h"
#include "move.h"
#include "u_arithmetic_logic.h"
#include "s_arithmetic_logic.h"
#include "f_arithmetic_logic.h"
#include "output.h"
#include "lecture.h"
#include "compare_jump.h"

void (*opcode_functions[])(CPU *, Instruction) = {
    loadu, loads, loadf,                      // 0, 1, 2
    loadv, loadt, loadg,                      // 3, 4, 5
    storeu, stores, storef,                   // 6, 7, 8
    storev, storet, storeg,                   // 9, 10, 11
    mov, movu, movs, movf, movv, movt, movg,  // 12, 13, 14, 15, 16, 17, 18
    movui, movsi, movfi, movvi, movti, movgi, // 19, 20, 21, 22, 23, 24
    cvtus, cvtsu, cvtuf, cvtfu, cvtsf, cvtfs, // 25, 26, 27, 28, 29, 30
    addu, subu, mulu, divu, modu,             // 31, 32, 33, 34, 35
    fmau, sqrtu, logu, incu, decu,            // 36, 37, 38, 39, 40
    andu, oru, xoru, shlu, shru,              // 41, 42, 43, 44, 45
    rolu, roru, popcntu, lmbu,                // 46, 47, 48, 49
    adds, subs, muls, divs,                   // 50, 51, 52, 53
    mods, fmas, sqrts, logs,                  // 54, 55, 56, 57
    ands, ors, xors, shls, shrs,              // 58, 59, 60, 61, 62
    rols, rors, popcnts, lmbs,                // 63, 64, 65, 66
    addf, subf, mulf, divf,                   // 67, 68, 69, 70
    fmaf_, sqrtf_, logf_,                     // 71, 72, 73
    cmpu, cmps, cmpf,                         // 74, 75, 76
    je, jne, jge, jl, jle, jz, jnz,           // 77, 78, 79, 80, 81, 82, 83
    outu, outs, outf, outa, outb, outx,       // 84, 85, 86, 87, 88, 89
    // hlt                                 // 90
};
