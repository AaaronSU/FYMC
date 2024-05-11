#include "../vm.h"
#include "../instruction.h"

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