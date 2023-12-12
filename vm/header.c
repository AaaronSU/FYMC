#include <stdio.h>
#include "header.h"
#include "type.h"

int read()
{
    FILE *file = fopen("test.bin", "rb"); // rb = 1 octet à la fois

    if (file == NULL)
    {
        perror("Erreur lors de l'ouverture du fichier");
        return 1;
    }
    u8 buffer;
    // Lit un octet depuis le fichier binaire
    while (fread(&buffer, sizeof(u8), 1, file) == 1)
    {
        printf("Octet lu : %02X\n", buffer);
        for (int i = 7; i >= 0; i--)
        {
            printf("%d", (buffer >> i) & 1);
        }
        printf("\n");
    }

    fclose(file);
    return 0;
}

void check()
{
}