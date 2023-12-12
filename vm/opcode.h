#ifndef OPCODE_H
#define OPCODE_H

enum Opcode
{
    // 1 - Memory

    // 1.1.1 - Scalar Load
    LOADU,
    LOADS,
    LOADF,
    // 1.1.2 - Vector Load
    LOADV,
    LOADT,
    LOADG,
    // 1.2.1 - Scalar Store
    STOREU,
    STORES,
    STOREF,
    // 1.2.2 - Vector Store
    STOREV,
    STORET,
    STOREG,

    // 2 - Data movement and type conversion

    // 2.1 - Data movement
    // 2.1.1 Move registre to an another
    MOV,
    MOVU,
    MOVS,
    MOVF,
    MOVV,
    MOVT,
    MOVG,
    // 2.1.2 Move Immediate to registre
    MOVUI,
    MOVSI,
    MOVFI,
    MOVVI,
    MOVTI,
    MOVGI,

    // 2.2 Type conversion
    CVTUS,
    CVTSU,
    CVTUF,
    CVTFU,
    CVTSF,
    CVTFS,

    // 3 - Unsigned arithmetic and logic
    ADDU,
    SUBU,
    MULU,
    DIVU,
    MODU,
    FMAU,
    SQRTU,
    LOGU,
    INCU,
    DECU,
    ANDU,
    ORU,
    XORU,
    SHLU,
    SHRU,
    ROLU,
    RORU,
    POPCNTU,
    LMBU,

    // 4 - Signed arithmetic and logic
    ADDS,
    SUBS,
    MULS,
    DIVS,
    MODS,
    FMAS,
    SQRTS,
    LOGS,
    // INCS,
    // DECS,
    ANDS,
    ORS,
    XORS,
    SHLS,
    SHRS,
    ROLS,
    RORS,
    POPCNTS,
    LMBS,

    // 5 - Floating-point arithmetic
    ADDF,
    SUBF,
    MULF,
    DIVF,
    FMAF,
    SQRTF,
    LOGF,

    // 6 - Comparaison and control flow
    // 6.1 Comparaison
    CMPU,
    CMPS,
    CMPF,

    // 6.2 Control flow
    JE,
    JNE,
    JGE,
    JL,
    JLE,
    JZ,
    JNZ,

    // 7 - Input/Output
    OUTU,
    OUTS,
    OUTF,
    OUTA,
    OUTB,
    OUTX,

    // 8 - Program termination
    HLT
};

#endif