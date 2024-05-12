#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#include "tools/tools.h"
#include "tools/types.h"
#include "parse/parse.h"
#include "assembly/assembly.h"

// TODO Make a function for error handling (make it more readable)

/// @brief Main function, used to launch big part of the asm to binary traduction
/// @param argc Number of arguments in the command line
/// @param argv Values of arguments in the command line
/// @return 0 if everything went well, 1 if something went wrong
i32 main(i32 argc, char** argv)
{
  if (argc != 2 && argc != 3)
  {
    printf("Invalid number of argument. Usage: %s [source] [destination] ; destination is optional.\n", argv[0]);
    return EXIT_FAILURE;
  }

  char* destination;
  if (argc == 2)
  {
    destination = malloc(8 * sizeof(char));
    memcpy(destination, "a.archy", 8);
  }
  else
  {
    u64 taille  = strlen(argv[2]) + 1;
    destination = malloc(taille * sizeof(char));
    if (destination == NULL)
    {
      perror("malloc in main");
      return EXIT_FAILURE;
    }

    memcpy(destination, argv[2], taille);
  }

  char*** tokens_list;
  i32*    tokens_list_sizes;
  FILE*   f;  // NOTE: tokenize function closes the file

  i64         taille_fichier;
  struct stat st;

  if (stat(argv[1], &st) == -1)
  {
      perror("stat in main");
      return EXIT_FAILURE;
  }
  // If is a dir
  if (!S_ISREG(st.st_mode))
  {
    free(destination);
    fprintf(stderr, "%s is a directory\n", argv[1]);
    return EXIT_FAILURE;
  }
  taille_fichier = st.st_size;

  f = fopen(argv[1], "r");
  if(f == NULL)
  {
    fprintf(stderr, "Could not open %s\n", argv[1]);
    perror("fopen in main");
    return EXIT_FAILURE;
  }

  tokens_list = calloc((u64)taille_fichier, sizeof(char**));
  if (tokens_list == NULL)
  {
    perror("malloc in main");
    return EXIT_FAILURE;
  }
  tokens_list_sizes = calloc((u64)taille_fichier, sizeof(int));
  if (tokens_list_sizes == NULL)
  {
    perror("malloc in main");
    return EXIT_FAILURE;
  }

  i32 nb_tokens = tokenize(f, tokens_list, tokens_list_sizes);
  if (nb_tokens == -1)
  {
    return EXIT_FAILURE;
  }


  // Opcodes
  char*** opcodes_list;
  i32*    opcodes_list_sizes;
  FILE*   f_op;

  if (stat("op_codes", &st) == -1)
  {
      perror("stat in main");
      return EXIT_FAILURE;
  }
  taille_fichier = st.st_size;

  f_op = fopen("op_codes", "r");
  if(f_op == NULL)
  {
    fprintf(stderr, "Could not open %s\n", "op_codes");
    perror("fopen in main");
    return EXIT_FAILURE;
  }

  opcodes_list = calloc((u64)taille_fichier, sizeof(char**));
  if (opcodes_list == NULL)
  {
    perror("malloc in main");
    return EXIT_FAILURE;
  }
  opcodes_list_sizes = calloc((u64)taille_fichier, sizeof(i32));
  if (opcodes_list_sizes == NULL)
  {
    perror("malloc in main");
    return EXIT_FAILURE;
  }

  i32 nb_tokens_op = tokenize(f_op, opcodes_list, opcodes_list_sizes);
  if (nb_tokens_op == -1)
  {
    return EXIT_FAILURE;
  }

  char*** registers;
  i32*    sizes_registers;
  i32     len_register;
  FILE*   f_reg;

  if (stat("register_list", &st) == -1)
  {
      perror("stat in parse");
      return EXIT_FAILURE;
  }
  taille_fichier = st.st_size;

  f_reg = fopen("register_list", "r");
  if(f_reg == NULL)
  {
    fprintf(stderr, "Could not open %s\n", "register_list");
    perror("fopen in parse");
    return EXIT_FAILURE;
  }

  registers       = calloc((u64)taille_fichier, sizeof(char**));
  sizes_registers = calloc((u64)taille_fichier, sizeof(i32));

  len_register = tokenize(f_reg, registers, sizes_registers);
  if (len_register == -1 || len_register == -3)
  {
    return EXIT_FAILURE;
  }
  else if (len_register == -2)
  {
    return EXIT_FAILURE;
  }


  i32 result;
  i32 data_start;
  i32 code_start;
  i32 indice_labels       = 0;
  i32 indice_req          = 0;
  char** labels           = calloc((u64)nb_tokens_op, sizeof(char**));
  char** requested_labels = calloc((u64)nb_tokens_op, sizeof(char**));
  result = parse(tokens_list,      tokens_list_sizes, nb_tokens,
                 &data_start,      &code_start,
                 opcodes_list,     nb_tokens_op,
                 labels,           &indice_labels,
                 requested_labels, &indice_req,
                 registers,        len_register);

  if (result == true)
  {
    write_stuff(destination,  tokens_list, tokens_list_sizes, nb_tokens,
                data_start,   code_start,
                opcodes_list, nb_tokens_op,
                registers,    len_register,
                requested_labels, indice_req);
  }
  else if (result == -2)
  {
    printf("Invalid syntax.\n");
    goto END;
  }
  else
  {
    printf("Invalid syntax.\n");
  }

  for (i32 i = 0; i < nb_tokens; ++i)
  {
    for (i32 j = 0; j < tokens_list_sizes[i]; ++j)
    {
     // printf("'%s' ", tokens_list[i][j]);
      free(tokens_list[i][j]);
    }
   // printf("\n");
    free(tokens_list[i]);
  }
  free(tokens_list);
  free(tokens_list_sizes);
  for (i32 i = 0; i < nb_tokens_op; ++i)
  {
    for (i32 j = 0; j < opcodes_list_sizes[i]; ++j)
    {
      // printf("'%s' ", opcodes_list[i][j]);
      free(opcodes_list[i][j]);
    }
    // printf("\n");
    free(opcodes_list[i]);
  }
  free(opcodes_list);
  free(opcodes_list_sizes);

  for (i32 i = 0; i < indice_labels; ++i)
  {
    free(labels[i]);
  }
  free(labels);
  for (i32 i = 0; i < indice_req; ++i)
  {
    free(requested_labels[i]);
  }
  free(requested_labels);

  for (i32 i = 0; i < len_register; ++i)
  {
    for (i32 j = 0; j < sizes_registers[i]; ++j)
    {
      free(registers[i][j]);
    }
    free(registers[i]);
  }
  free(registers);
  free(sizes_registers);

  END:
  free(destination);
  // printf("%d\n", result);

  return EXIT_SUCCESS;
}
