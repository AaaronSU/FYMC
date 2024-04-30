#ifndef LECTURE_H
#define LECTURE_H
#include "type.h"

struct Header
{
    char magic_number[8];
    u64 address_data_section;
    u64 data_section_size; // in bytes
    u64 address_code_section;
    u64 code_section_size;
    u64 total_binary_file_size; // in bytes
};

struct Header get_header_binary_file(char *file_name);
char *lecture_fichier_binaire(char *file_name, struct Header file_header);

#endif