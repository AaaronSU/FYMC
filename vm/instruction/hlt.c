#include "../vm.h"
#include "../instruction.h"

void hlt(core_t *core)
{
    // info("Core %d successful finished\n", core->id);
    core->IP += SIZE_INSTRUCTION_IN_BYTE;

    // exit(0); // à enlever pour parallélisme, sinon chut down dès un truc fini
}

void undefined_instruction(core_t *core)
{
    warn("Instruction Set don't defined the following instruction: %d. (maybe yet)", *(u8 *)&(core->file_buffer[core->IP]));
    exit(0);
}
