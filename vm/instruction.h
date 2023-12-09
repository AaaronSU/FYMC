#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "init.h"
// offset -> u64 ou i64 ? voir plus petit nombre
void loadu(u64 u2, u64 u0, u64 u1, i64 offset);
void loads(i64 s0, u64 u0, u64 u1, i64 offset);
void loadf(f64 f0, u64 u0, u64 u1, i64 offset);

// Data movement

// scalaire - scalaire 
void movu(u8 u0, u8 u1);
void movs(u8 s0, u8 s1);
void movf(u8 f0, u8 f1);

// vecteur - vecteur
void movv(u8 v0, u8 v1);
void movt(u8 t0, u8 t1);
void movg(u8 g0, u8 g1);

// scalaire - immédiate
void movui(u8 u0, u64 imm);
void movsi(u8 s0, i64 imm);
void movfi(u8 f0, f64 imm);

// vecteur - immédiate
void movvi(u8 v0, u8* tab_imm[]);
void movti(u8 t0, u8* tab_imm[]);
void movgi(u8 g0, u8* tab_imm[]);

// A changer, c'est des indices de taille 5 bits. (adresse) 
// Gestion des erreurs, la taille des indices etc
// fonction redondante, essayer de généraliser

#endif