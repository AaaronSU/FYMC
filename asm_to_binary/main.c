#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "tools/tools.h"
#include "parse/parse.h"
#include "assembly/assembly.h"

/// @brief Main function, used to launch big part of the asm to binary traduction
/// @param argc Number of arguments in the command line
/// @param argv Values of arguments in the command line
/// @return 0 if everything went well, 1 if something went wrong
int main(int argc, char** argv)
{
    //Is argv[1] is a valid file ?
    if(file_verification(argc, argv) == FALSE)
    {
        return EXIT_FAILURE;
    }

    char*** op_name_list; //Where the op name and specification are stored
    op_name_list = tokenize("op_codes");

    char*** register_list; //Where the registers infos are stored
    register_list = tokenize("register_list");

    //Loading the code in memory for faster accesses
    // NOTE: Yeah we might do that, but right now no
    // char* loaded_file = load_file(argv[1]);


    // char str[] = "add;U1;U2";
    // correct_op_code(retreive_token(str,';'),op_name_list[0],register_list);


    long long int nb_code = 0;
    long long int nb_data = 0;

    nb_ligne_section(argv[1], &nb_data, &nb_code);

    char*** data_array = malloc(nb_data);
    char*** code_array = malloc(nb_code);

    // NOTE: Maybe something will go wrong with pointers, const and stuff idk
    // TODO: test parse (normal cases, empty sections, secctions of size 1 and other weird cases)
    bool parsing_went_alright = parse(argv[1], data_array, code_array,
                                        &nb_data, &nb_code,
                                        op_name_list, register_list);

    printf("%d\n", parsing_went_alright);
    free(data_array);
    free(code_array);



    // //We now read how many lines we have in data and how many lines we have in code
    // int data_lines, code_lines;
    // if((data_lines = data_infos(loaded_file)) == -1)
    // {
    //     fprintf(stderr,"Error : missing data label.\n");
    //     return EXIT_FAILURE;
    // }
    //
    // if((code_lines = code_infos(loaded_file)) == -1)
    // {
    //     fprintf(stderr,"Error : missing code label.\n");
    //     return EXIT_FAILURE;
    // }



    // char temp[1024];
    // char str[] = "add U1,   U2   \"h;;;e;ytest\"#izi";
    // printf("original string : %s\n",str);
    // add_semicolon(str);
    // remove_space(str,temp);
    // char** tokens = retreive_token(temp,';');
    // print_tokens_line(tokens);




    // int place;
    // char str[] = "add;U1;U2";
    // char** tokens = retreive_token(str,";");

    // //Ici boucle for qui vérifie chaque ligne
    // {
    //     if((place = detect_op_code(tokens[0],op_name_list)) == -1)
    //     {
    //         fprintf(stderr,"Error : the op code %s does not exist.\n",tokens[0]);
    //         return EXIT_FAILURE;
    //     }

    //     if(!correct_op_code(tokens,op_name_list[place],register_list))
    //     {
    //         printf("Error : args not correct\n");
    //     }
    // }











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

    return EXIT_SUCCESS;
}







/*
LISTE DES FONC :

1) load_file(str) : charge en mémoire l'intégralité du fichier OK
2) load_op_code() : charge en mémoire les données des op code OK

3) char* add_comma(str) = ajoute une virgule après le premier "mots" OK
4) char* remove_space(str) = supprime l'intégralité des espaces OK
5) char** retreive_token(str,char) = découpe la chaine en token avec , comme séparateur OK

    = donne quelque chose comme token["add","U1","U2",NULL]; (ne pas oublier le NULL qui indique la fin des tokens)

6) bool detect_alias(char**) = vérifie s'il y a un token qui termine par : OK
7) bool correct_alias(char**) = vérifie s'il l'alias est correct (aka présent à l'indice 0 et pas d'autres tokens) OK

8) bool detect_op_code(char*) = vérifie que token[0] correspond au nom d'un op code
9) bool correct_op_code(char**) = vérifie le bon nombre d'arguments ainsi que le bon nom des registres

    10) bool correct_register_name(char**) = verifie que les noms de registres existent

11) char** parse(str) : s'occupe de vérifie l'intrégité de la ligne (partie code pas data pour l'instant).

        parse appelera parse_data puis parse_code
        parse_code contiendra les fonctions de 3 à 10 et parse_data en réutilisera certaines

*/
