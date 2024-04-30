#include <stdio.h>
#include <output.h>

void outu(CPU *cpu, Instruction inst)
{
    printf("%llu\n", cpu->U[inst.destination]);
    cpu->IP += INSTRUCTION_BYTES;
}

void outs(CPU *cpu, Instruction inst)
{
    printf("%lld\n", cpu->U[inst.destination]);
    cpu->IP += INSTRUCTION_BYTES;
}

void outf(CPU *cpu, Instruction inst)
{
    printf("%f\n", cpu->F[inst.destination]);
    cpu->IP += INSTRUCTION_BYTES;
}

void outa(CPU *cpu, Instruction inst)
{
    printf("%c\n", (char)(cpu->U[inst.destination] & 0xFF));
    cpu->IP += INSTRUCTION_BYTES;
}

// Function to print a string of ASCII characters from the memory address stored in *U0*
// Stops when 0 is encountered
// Argument :
//      - U : registre where store adress of string
//      - buffer : buffer where store data

// Need to add overflow memory checking.
void outb(CPU *cpu, Instruction inst)
{
    u64 adress = cpu->U[inst.destination];
    int adress_value = cpu->BinaryFile[adress / 8];

    int i = 0;
    while (adress_value)
    {
        printf("%c", adress_value);
        i++;
        adress_value = cpu->BinaryFile[adress / 8 + i];
    }
    cpu->IP += INSTRUCTION_BYTES;
}

void outx(CPU *cpu, Instruction inst)
{
    printf("0x%llX\n", cpu->U[inst.destination]);
    cpu->IP += INSTRUCTION_BYTES;
}