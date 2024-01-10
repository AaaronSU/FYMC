#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "tools/tools.h"
#include "parse/parse.h"
#include "assembly/assembly.h"

/// @brief Main function, used to launch big part of the asm to binary traduction
/// @param argc Number of arguments in the command line
/// @param argv Values of arguments in the command line
/// @return 0 if everything went well, 1 if something went wrong
int main(int argc, char** argv)
{
    //Is argv[1] a valid file ?
    if(file_verification(argc, argv) == FALSE)
    {
        return EXIT_FAILURE;
    }

    long long int nb_code = 0;
    long long int nb_data = 0;

    nb_ligne_section(argv[1], &nb_data, &nb_code);
    if (nb_code <= 0 || nb_data < 0)
    {
        fprintf(stderr, "No Code Section!\n");
        //TODO: exit properly
        return EXIT_FAILURE;
    }

    //NOTE calloc of size nb+1 so that memory is mine, but last elt is NULL
    // (set to NULL is parse)
    char*** code_array = calloc(nb_code + 1, sizeof(char**));
    for (size_t i = 0; i < nb_code; ++i)
    {
        code_array[i] = calloc(128, sizeof(char*));
        for(size_t j = 0; j < 128; ++j)
        {
            code_array[i][j] = calloc(128, sizeof(char));
        }
    }

    char*** data_array;
    if (nb_data > 0)
        {
            data_array = calloc(nb_data + 1, sizeof(char**));
            for (size_t i = 0; i <nb_data; ++i)
            {
                data_array[i] = calloc(128, sizeof(char*));
                for(size_t j = 0; j < 128; ++j)
                {
                    data_array[i][j] = calloc(128, sizeof(char));
                }
            }
        }

    //Where the op name and specification are stored
    char*** op_name_list = tokenize("op_codes");


    //Where the registers infos are stored
    char*** register_list = tokenize("register_list");

    char** address_array = calloc(nb_code + 1, sizeof(char**));
    for (size_t i = 0; i <nb_code; ++i)
    {
        address_array[i] = calloc(2, sizeof(long int));
    }
    // NOTE: Probably no alias if 1 code line ; untested case but shouldn't happen
    // TODO: Make a special case for note above
    free(address_array[nb_code]);
    address_array[nb_code] = NULL;
    strcpy(address_array[0], "0");

    // NOTE: Maybe something will go wrong with pointers, const and stuff idk
    // TODO: test parse (normal cases, empty sections, sections of size 1 and other weird cases)
    bool parsing_went_alright = parse(argv[1], data_array, code_array,
                                        &nb_data, &nb_code,
                                        op_name_list, register_list, address_array);

    printf("Parsing result: %d\n", parsing_went_alright);
    // printf("%c\n", code_array[0][0][0]);

    // print_tokens_list(code_array);
    // print_tokens_list(data_array);

    if (parsing_went_alright)
        write_stuff("file_test", data_array, code_array,
                    &nb_data, &nb_code, op_name_list, register_list, address_array);

    free_char3(op_name_list, 128, 128);
    free_char3(register_list, 128, 128);
    if (nb_data > 0)
        free_char3(data_array, nb_data, 128);
    free_char3(code_array, nb_code, 128);
    free_char2(address_array, nb_code);

/*
    //We load the file to assemble in memory
    char* file = load_file(argv[1]);

    //print_file(file);

    //Initialization of operand code behaviour
    // if(parse() == FALSE)
    // {
    //     return EXIT_FAILURE;
    // }

    // assembly();

    //We dont need the file loaded in memory anymore
    free(file);
*/

    return EXIT_SUCCESS;
}
