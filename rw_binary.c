#include <stdio.h>
#include <stdlib.h>

// La fonction écriture traduit le code suivant en binaire
// et l'enregistre dans notre_ultime_fichier_binaire.

// La fonction de son côté lit le fichier binaire et
// imprime sur l'écran les données.

// # This is an assembly hello world program

// #The data section holding the program's constants
// data : ascii msg "Hello, World!\n"

// #The code section
//        code : movui u0,
//               @msg
//                   outb u0
//                       hlt

typedef long long u64;

struct Header
{
    char magic_number[8];
    u64 address_data_section;
    u64 data_section_size; // in bytes
    u64 address_code_section;
    u64 code_section_size;
    u64 total_binary_file_size; // in bytes
};

void ecriture()
{
    // Header à écrire dans le fichier binaire
    struct Header donnees_ecrites = {
        .magic_number = {'A', 'R', 'C', 'H', 'Y', '0', '.', '0'},
        .address_data_section = 0,
        .data_section_size = 0,
        .address_code_section = 0,
        .code_section_size = 0,
        .total_binary_file_size = sizeof(struct Header),
    };

    // Ouverture du fichier en écriture binaire
    FILE *fichier = fopen("notre_ultime_fichier_binaire", "wb");

    if (fichier == NULL)
    {
        // fprintf(stderr, "Erreur lors de l'ouverture du fichier en écriture.\n");
        perror("Erreur lors de l'ouverture du fichier en lecture");
    }

    // Écriture des données dans le fichier binaire
    fwrite(&donnees_ecrites, sizeof(struct Header), 1, fichier);

    // Hello world!\n
    // 72 101 108 108 111 44 32 87 111 114 108 100
    const char data_suite[] = {
        0b01001000, 0b01100101, 0b01101100, 0b01101100, 0b01101111, 0b00101100, 0b00100000,
        0b01010111, 0b01101111, 0b01110010, 0b01101100, 0b01100100, 0b00100001, 0b1010, 0b0};
    fwrite(data_suite, sizeof(char), sizeof(data_suite), fichier);

    // Met à jour data_section_size dans Header
    donnees_ecrites.data_section_size = sizeof(data_suite);

    const char code_suite[] = {
        0b0010011,
        0b0,
        0b0,
        0b0,
        // en haut Movui u0
        0b0,
        0b0,
        0b0,
        0b0,
        0b0,
        0b0,
        0b11,
        0b0000000,
        // en haut immediate 64 bits
        0b1011011,
        0b0,
        0b0,
        0b0,
        // en haut Outb u0
        0b01011101,
        0b0,
        0b0,
        0b0,
        // en haut hlt

    };
    // Écriture des données dans le fichier binaire
    fwrite(code_suite, sizeof(char), sizeof(code_suite), fichier);

    donnees_ecrites.code_section_size = sizeof(code_suite);
    donnees_ecrites.address_code_section = sizeof(data_suite) + sizeof(struct Header);

    donnees_ecrites.total_binary_file_size = sizeof(data_suite) + sizeof(struct Header) + sizeof(code_suite);

    // Repositionne le curseur pour mettre à jour le Header dans le fichier
    fseek(fichier, 0, SEEK_SET);
    fwrite(&donnees_ecrites, sizeof(struct Header), 1, fichier);

    // Fermeture du fichier
    fclose(fichier);
}

char *lecture()
{
    // Ouverture du fichier en lecture binaire
    FILE *fichier = fopen("notre_ultime_fichier_binaire", "rb");

    if (fichier == NULL)
    {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier en lecture.\n");
        return (void *)1;
    }

    // Lecture des données depuis le fichier binaire
    struct Header taille_fichier;
    if (fread(&taille_fichier, sizeof(struct Header), 1, fichier) != 1)
    {
        fprintf(stderr, "Erreur lors de la lecture des données du fichier.\n");
        fclose(fichier);
        return (void *)1;
    }

    // Affiche les informations lues depuis le fichier
    printf("Magic Number: %.8s\n", taille_fichier.magic_number);
    printf("Address Data Section: %lld\n", taille_fichier.address_data_section);
    printf("Data Section Size: %lld bytes\n", taille_fichier.data_section_size);
    printf("Address Code Section: %lld\n", taille_fichier.address_code_section);
    printf("Code Section Size: %lld bytes\n", taille_fichier.code_section_size);
    printf("Total Binary File Size: %lld bytes\n", taille_fichier.total_binary_file_size);

    // Fermeture du fichier

    fseek(fichier, 0, SEEK_SET);

    char *buffer = (char *)malloc(taille_fichier.total_binary_file_size);

    if (buffer == NULL)
    {
        fprintf(stderr, "Erreur lors de l'allocation du buffer de lecture.\n");
        fclose(fichier);
        return (void *)1;
    }

    // Lit le fichier complet dans le buffer
    if (fread(buffer, 1, taille_fichier.total_binary_file_size, fichier) != taille_fichier.total_binary_file_size)
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
    for (int i = 0; i < taille_fichier.total_binary_file_size; ++i)
    {
        printf("%u ", buffer[i]); // Affichage de l'octet en hexadécimal
    }
    printf("\n");
    return buffer;
}

int main()
{
    ecriture();
    char *buffer = lecture();
    // Libère le buffer
    free(buffer);

    return 0;
}
