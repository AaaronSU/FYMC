#include "../vm.h"
#include "../instruction.h"

void je(core_t *core)
{
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
    u64 immediate = get_immediate(core);
    DEBUG_PRINT("--------Avant JE--------\n");
    DEBUG_PRINT("Instruction Pointer a pour valeur %ld\n", core->IP);
    core->IP = core->CF[7] ? immediate : core->IP;
    DEBUG_PRINT("--------Après JE--------\n");
    DEBUG_PRINT("Instruction Pointer a pour valeur %ld\n\n", core->IP);
}

void jne(core_t *core)
{
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
    u64 immediate = get_immediate(core);
    DEBUG_PRINT("--------Avant JNE--------\n");
    DEBUG_PRINT("Instruction Pointer a pour valeur %ld\n", core->IP);
    core->IP = core->CF[6] ? immediate : core->IP;
    DEBUG_PRINT("--------Après JNE--------\n");
    DEBUG_PRINT("Instruction Pointer a pour valeur %ld\n\n", core->IP);
}

void jge(core_t *core)
{
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
    u64 immediate = get_immediate(core);
    DEBUG_PRINT("--------Avant JGE--------\n");
    DEBUG_PRINT("Instruction Pointer a pour valeur %ld\n", core->IP);
    core->IP = core->CF[5] | core->CF[4] ? immediate : core->IP;
    DEBUG_PRINT("--------Après JGE--------\n");
    DEBUG_PRINT("Instruction Pointer a pour valeur %ld\n\n", core->IP);
}

void jl(core_t *core)
{
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
    u64 immediate = get_immediate(core);
    DEBUG_PRINT("--------Avant JL--------\n");
    DEBUG_PRINT("Instruction Pointer a pour valeur %ld\n", core->IP);
    core->IP = core->CF[3] ? immediate : core->IP;
    DEBUG_PRINT("--------Après JL--------\n");
    DEBUG_PRINT("Instruction Pointer a pour valeur %ld\n\n", core->IP);
}

void jle(core_t *core)
{
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
    u64 immediate = get_immediate(core);
    DEBUG_PRINT("--------Avant JLE--------\n");
    DEBUG_PRINT("Instruction Pointer a pour valeur %ld\n", core->IP);
    core->IP = core->CF[2] ? immediate : core->IP;
    DEBUG_PRINT("--------Après JLE--------\n");
    DEBUG_PRINT("Instruction Pointer a pour valeur %ld\n\n", core->IP);
}