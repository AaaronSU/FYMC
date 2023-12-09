#include <stdio.h>
#include <stdint.h>

#include "instruction.h"
#include "init.h"


void movu(u8 u0, u8 u1) 
{
    U[u0] = U[u1]; // opération
    IP += SIZE_INSTRUCTION; // bouge IP de 32 bits  
}

void movs(u8 s0, u8 s1)
{
    S[s0] = S[s1];
    IP += SIZE_INSTRUCTION;
}

void movf(u8 f0, u8 f1) 
{
    F[f0] = F[f1];
    IP += SIZE_INSTRUCTION;
}

// vecteur - vecteur

void movv(u8 v0, u8 v1)
{
    for(int i; i != SIZE_VECTOR; i++)
    {
        V[v0][i] = V[v1][i];
    }
    IP += SIZE_INSTRUCTION;
}

void movt(u8 t0, u8 t1) 
{
    for(int i; i != SIZE_VECTOR; i++)
    {
        T[t0][i] = T[t1][i];
    }
    IP += SIZE_INSTRUCTION;
}

void movg(u8 g0, u8 g1) 
{
    for(int i; i != SIZE_VECTOR; i++)
    {
        G[g0][i] = G[g1][i];
    }
    IP += SIZE_INSTRUCTION;
}

// scalaire - immédiate
void movui(u8 u0, u64 imm) 
{
    U[u0] = imm;
    IP += SIZE_INSTRUCTION;
}

void movsi(u8 s0, i64 imm)
{
    S[s0] = imm;
    IP += SIZE_INSTRUCTION;
}

void movfi(u8 f0, f64 imm)
{
    F[f0] = imm;
    IP += SIZE_INSTRUCTION;
}

/*
// vecteur - immédiate

void movvi(u8 v0, ascii* tab_imm[])
{
    for(int i; i != SIZE_VECTOR; i++)
    {
        T[v0][i] = tab_imm[i];
    }
    IP += SIZE_INSTRUCTION;
}
void movti(u8 t0, ascii* tab_imm[]) 
{
    for(int i; i != SIZE_VECTOR; i++)
    {
        T[t0][i] = tab_imm[i];
    }
    IP += SIZE_INSTRUCTION;
}
void movgi(u8 g0, ascii* tab_imm[])
{
    for(int i; i != SIZE_VECTOR; i++)
    {
        T[g0][i] = tab_imm[i];
    }
    IP += SIZE_INSTRUCTION;
}
*/