#include <stdio.h>
#include <stdlib.h>
#include <lecture.h>

struct Header get_header_binary_file(char *file_name)
{
    FILE *fichier = fopen(file_name, "rb");
    if (fichier == NULL)
    {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier en lecture.\n");
    }
    struct Header file_header;
    if (fread(&file_header, sizeof(struct Header), 1, fichier) != 1)
    {
        fprintf(stderr, "Erreur lors de la lecture des données du fichier.\n");
        fclose(fichier);
    }
    fclose(fichier);
    return file_header;
}

char *lecture_fichier_binaire(char *file_name, struct Header file_header)
{
    FILE *fichier = fopen(file_name, "rb");
    // Repositionne le curseur dans le fichier
    fseek(fichier, 0, SEEK_SET);

    // printf("magic number : %s\n", file_header.magic_number);
    // printf("address data section: %lld\n", file_header.address_data_section);
    // printf("data section size: %lld octets\n", file_header.data_section_size);
    // printf("address code section: %lld\n", file_header.address_code_section);
    // printf("code section size: %lld octets\n", file_header.code_section_size);
    // printf("total binary file size: %lld octets\n", file_header.total_binary_file_size);

    char *buffer = (char *)malloc(file_header.total_binary_file_size);
    if (buffer == NULL)
    {
        fprintf(stderr, "Erreur lors de l'allocation du buffer de lecture.\n");
        fclose(fichier);
        return (void *)1;
    }

    // Lit le fichier complet dans le buffer
    if (fread(buffer, 1, file_header.total_binary_file_size, fichier) != file_header.total_binary_file_size)
    {
        fprintf(stderr, "Erreur lors de la lecture du fichier complet.\n");
        fclose(fichier);
        free(buffer);
        return (void *)1;
    }

    fclose(fichier);
    return buffer;
}