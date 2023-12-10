#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include <stddef.h>
#include "init.h"
#include "type.h"
// offset -> u64 ou i64 ? voir plus petit nombre
void loadu(u64 u2, u64 u0, u64 u1, i64 offset);
void loads(i64 s0, u64 u0, u64 u1, i64 offset);
void loadf(f64 f0, u64 u0, u64 u1, i64 offset);

// Data movement
void mov(CPU* cpu, u8 destination, u8 source, u8 type_1, u8 type_2);
// scalaire - scalaire 
void movu(u8 u0, u8 u1, CPU* cpu);
void movs(u8 s0, u8 s1, CPU* cpu);
void movf(u8 f0, u8 f1, CPU* cpu);

// vecteur - vecteur
void movv(u8 v0, u8 v1, CPU* cpu);
void movt(u8 t0, u8 t1, CPU* cpu);
void movg(u8 g0, u8 g1, CPU* cpu);

// scalaire - immédiate
void movui(u8 u0, u64 imm, CPU* cpu);
void movsi(u8 s0, i64 imm, CPU* cpu);
void movfi(u8 f0, f64 imm, CPU* cpu);

// vecteur - immédiate
void movvi(u8 v0, u8* imm, CPU* cpu);
void movti(u8 t0, u8* imm, CPU* cpu);
void movgi(u8 g0, u8* imm, CPU* cpu);

// A changer, c'est des indices de taille 5 bits. (adresse) 
// Gestion des erreurs, la taille des indices etc
// fonction redondante, essayer de généraliser

#endif