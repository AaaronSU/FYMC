#include <stdio.h>
#include <stdint.h>

#include "instruction.h"
#include "init.h"

// Définition des fonctions mov pour les différentes opérations

void movu(u8 u0, u8 u1, CPU* cpu) {
    cpu->U[u0] = cpu->U[u1];
    cpu->IP += SIZE_INSTRUCTION;
}

void movs(u8 s0, u8 s1, CPU* cpu) {
    cpu->S[s0] = cpu->S[s1];
    cpu->IP += SIZE_INSTRUCTION;
}

void movf(u8 f0, u8 f1, CPU* cpu) {
    cpu->F[f0] = cpu->F[f1];
    cpu->IP += SIZE_INSTRUCTION;
}

void movv(u8 v0, u8 v1, CPU* cpu) {
    for (int i = 0; i < SIZE_VECTOR; i++) {
        cpu->V[v0][i] = cpu->V[v1][i];
    }
    cpu->IP += SIZE_INSTRUCTION;
}

void movt(u8 t0, u8 t1, CPU* cpu) {
    for (int i = 0; i < SIZE_VECTOR; i++) {
        cpu->T[t0][i] = cpu->T[t1][i];
    }
    cpu->IP += SIZE_INSTRUCTION;
}

void movg(u8 g0, u8 g1, CPU* cpu) {
    for (int i = 0; i < SIZE_VECTOR; i++) {
        cpu->G[g0][i] = cpu->G[g1][i];
    }
    cpu->IP += SIZE_INSTRUCTION;
}

void movui(u8 u0, u64 imm, CPU* cpu) {
    cpu->U[u0] = imm;
    cpu->IP += SIZE_INSTRUCTION;
}

void movsi(u8 s0, i64 imm, CPU* cpu) {
    cpu->S[s0] = imm;
    cpu->IP += SIZE_INSTRUCTION;
}

void movfi(u8 f0, f64 imm, CPU* cpu) {
    cpu->F[f0] = imm;
    cpu->IP += SIZE_INSTRUCTION;
}

void movvi(u8 v0, u8* imm, CPU* cpu) {
    for (int i = 0; i < SIZE_VECTOR; i++) {
        cpu->V[v0][i] = imm[i];
    }
    cpu->IP += SIZE_INSTRUCTION;
}

void movti(u8 t0, u8* imm, CPU* cpu) {
    for (int i = 0; i < SIZE_VECTOR; i++) {
        cpu->T[t0][i] = imm[i];
    }
    cpu->IP += SIZE_INSTRUCTION;
}

void movgi(u8 g0, u8* imm, CPU* cpu) {
    for (int i = 0; i < SIZE_VECTOR; i++) {
        cpu->G[g0][i] = imm[i];
    }
    cpu->IP += SIZE_INSTRUCTION;
}