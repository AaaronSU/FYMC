#include <stdio.h>
#include <stdlib.h>
#include <lecture.h>

struct Header get_header_binary_file(char *file_name)
{
    // Ouverture du fichier en lecture binaire
    FILE *fichier = fopen(file_name, "rb");

    if (fichier == NULL)
    {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier en lecture.\n");
    }

    // Lecture des données depuis le fichier binaire
    struct Header file_header;
    if (fread(&file_header, sizeof(struct Header), 1, fichier) != 1)
    {
        fprintf(stderr, "Erreur lors de la lecture des données du fichier.\n");
        fclose(fichier);
    }

    // Affiche les informations lues depuis le fichier
    printf("Magic Number: %.8s\n", file_header.magic_number);
    printf("Address Data Section: %lld\n", file_header.address_data_section);
    printf("Data Section Size: %lld bytes\n", file_header.data_section_size);
    printf("Address Code Section: %lld\n", file_header.address_code_section);
    printf("Code Section Size: %lld bytes\n", file_header.code_section_size);
    printf("Total Binary File Size: %lld bytes\n", file_header.total_binary_file_size);
    fclose(fichier);
    return file_header;
}

char *lecture_fichier_binaire(char *file_name, struct Header file_header)
{
    FILE *fichier = fopen(file_name, "rb");
    // Repositionne le curseur dans le fichier
    fseek(fichier, 0, SEEK_SET);

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

    // Fermeture du fichier
    fclose(fichier);

    // Affiche le contenu du buffer octet par octet avec un espace entre deux octets
    printf("Contenu du buffer :\n");
    for (int i = 0; i < file_header.total_binary_file_size; ++i)
    {
        printf("%u ", buffer[i]); // Affichage de l'octet en hexadécimal
    }
    printf("\n");
    return buffer;
}