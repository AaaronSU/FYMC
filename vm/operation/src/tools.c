#include "tools.h"
#include "cpu.h"

Instruction parse_instruction(u32 instruction)
{
    Instruction inst;
    inst.opcode = instruction >> (SIZE_INSTRUCTION - 8) & 0xFF;
    inst.unused = instruction >> (SIZE_INSTRUCTION - 17) & 0x1FF;
    inst.destination = instruction >> (SIZE_INSTRUCTION - 22) & 0x1F;
    inst.source_1 = instruction >> (SIZE_INSTRUCTION - 27) & 0x1F;
    inst.source_2 = instruction & 0x1F;
    return inst;
}

u8 get_offset(u16 unused)
{
    // les 8 bits de poids faible
    return unused & 0xFF;
}