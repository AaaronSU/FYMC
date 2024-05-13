#include "../vm.h"
#include "../instruction.h"

void cvtus(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    core->S[instruction.register_1] = (i64)core->U[instruction.register_2];
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

void cvtsu(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    core->U[instruction.register_1] = (u64)core->S[instruction.register_2];
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

void cvtuf(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    core->F[instruction.register_1] = (f64)core->U[instruction.register_2];
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

void cvtfu(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    core->U[instruction.register_1] = (u64)core->F[instruction.register_2];
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

void cvtsf(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    core->F[instruction.register_1] = (f64)core->F[instruction.register_2];
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}

void cvtfs(core_t *core)
{
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    core->S[instruction.register_1] = (i64)core->F[instruction.register_2];
    core->IP += SIZE_INSTRUCTION_IN_BYTE;
}