#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "opcode.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Error: Incorrect number of arguments\n");
        fprintf(stderr, "Usage: %s <path>\n", argv[0]);
        return 1;
    }
    CPU cpu;
    memset(&cpu, 0, sizeof(CPU)); // Initialize cpu to zero
    struct Header header = get_header_binary_file(argv[1]);
    cpu.BinaryFile = lecture_fichier_binaire(argv[1], header);
    cpu.IP = header.address_code_section;

    while (cpu.IP < header.total_binary_file_size)
    {
        char *opcode_byte = &cpu.BinaryFile[cpu.IP];
        int opcode = (int)(*opcode_byte);
        if (opcode == 90)
        {
            free(cpu.BinaryFile);
            return 0;
        }
        Instruction inst = parse_instruction(cpu.BinaryFile[cpu.IP]);
        opcode_functions[opcode](&cpu, inst);
    }

    free(cpu.BinaryFile); // Attention : ceci n'est pas un double free !!!
    fprintf(stderr, "Error: The binary file does not terminate correctly.\n");

    return 1;
}