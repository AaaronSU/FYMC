#include <stdio.h>
#include <output.h>

void outu(CPU *cpu, Instruction inst)
{
    printf("%llu\n", cpu->U[inst.destination]);
    cpu->IP += SIZE_INSTRUCTION;
}

void outs(CPU *cpu, Instruction inst)
{
    printf("%lld\n", cpu->U[inst.destination]);
    cpu->IP += SIZE_INSTRUCTION;
}

void outf(CPU *cpu, Instruction inst)
{
    printf("%f\n", cpu->F[inst.destination]);
    cpu->IP += SIZE_INSTRUCTION;
}

void outa(CPU *cpu, Instruction inst)
{
    printf("%c\n", (char)(cpu->U[inst.destination] & 0xFF));
    cpu->IP += SIZE_INSTRUCTION;
}

// Function to print a string of ASCII characters from the memory address stored in *U0*
// Stops when 0 is encountered
// Argument :
//      - U : registre where store adress of string
//      - buffer : buffer where store data

// Need to add overflow memory checking.
void outb(CPU *cpu, Instruction inst)
{
    printf("welcome\n");
    // printf("%d", inst.destination);
    u64 adress = cpu->U[inst.destination];
    printf("value: %d\n", adress);

    int adress_value = cpu->BinaryFile[adress / 8];
    // printf("value adress: %c\n", adress_value);

    int i = 0;
    while (adress_value)
    {
        printf("%c", adress_value);
        i++;
        adress_value = cpu->BinaryFile[adress / 8 + i];
    }
    // int value = (int)*value_adress;

    // int i = 0;
    // int immediat = 0;
    // while (i < 8)
    // {
    //     char *value_adress = &(cpu->BinaryFile[adress]);
    //     int value = (int)*value_adress;
    //     immediat += value << (8 - 1);
    //     printf("value: %d, immediat= %d\n", value, immediat);
    //     i++;
    //     cpu->IP += 1;
    // }

    // while (str != 0)
    // {
    //     printf("%c", str);
    //     str++;
    // }
    // printf("\n");
    cpu->IP += 4;
}

void outx(CPU *cpu, Instruction inst)
{
    printf("0x%llX\n", cpu->U[inst.destination]);
    cpu->IP += SIZE_INSTRUCTION;
}