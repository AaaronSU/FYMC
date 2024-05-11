#define _DEFAULT_SOURCE

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <endian.h>
#include <errno.h>

// #include "../parse/parse_code/parse_code.h"
// #include "../parse/parse_data/parse_data.h"
// #include "../parse/parse.h"
#include "../tools/tools.h"
#include "../tools/types.h"

#define header_size 10

i64 get_number_of_one(char* mask)
{
  u64 taille = strlen(mask);
  i64 return_value = 0;
  for (u64 i = 2; i < taille; ++i)
  {
    if (mask[i] == '1')
    {
      ++return_value;
    }
  }
  return return_value;
}


u64 get_max(char* mask)
{
  u64 taille = strlen(mask);
  u64 return_value = 0;
  for (u64 i = 2; i < taille; ++i)
  {
    if (mask[i] == '1')
    {
      return_value = (taille - i);
      break;
    }
  }
  return return_value;
}


u64 retreive_value(char* mask)
{
  u64 taille = strlen(mask);
  u64 return_value = 0;
  for (u64 i = 2; i < taille; ++i)
  {
    if (mask[i] == '1')
    {
      return_value += 1 << (taille - i - 1);
    }
  }
  return return_value;
}


// Checks if ligne[0] is in address_array
bool its_an_alias(char* ligne, char** address_array, i32* i, i32 address_size)
{
  for (*i=0; *i < address_size; *i = *i + 1)
  // for (u64 i = 1; address_array[i] != NULL; ++i)
  {
    // Alias gotta be first in line
    if (strcmp(ligne, address_array[*i]) == 0)
    {
      return true;
    }
  }
  return false;
}


i32 get_op_position(char* to_find, char*** op_name_list, i32 size)
{
  i32 position = -1;
  for (i32 i = 0; i < size; ++i)
  {
    if (strcasecmp(to_find, op_name_list[i][0]) == 0)
      {
        position = i;
        break;
      }
  }
  return position;
}


void get_threads_position(char*** tokens_list,  i64  start,       char*** op_name_list,
                          i32     op_list_size, u64* thread_rank, u64*    out)
{
  u64 i = (u64)start;
  bool has_written = false;
  i32 position = get_op_position(tokens_list[i][0],
                                 op_name_list, op_list_size);
  while (position != 92)
  {
    position = get_op_position(tokens_list[i++][0],
                               op_name_list, op_list_size);
    if (position == 93)
    {
      if (has_written == false)
      {
        out[0] = *thread_rank;
        has_written = true;
      }
      *thread_rank += 1;
    }
  }
  out[1] = *thread_rank;
  *thread_rank += 1;
  out[2] = i;
  return;
}


// Returns the 32bits sequence we need to write for ligne
void code_to_number(char*** tokens_list,    i32* tokens_sizes,
                    i32     code_start,     i32  indice,
                    char**  address_array,  u64* address_indices,
                    char*** op_name_list,   i32  op_list_size,
                    i32*    to_write,       i64* immediate_value,
                    char*** register_list,  i32 reg_size,
                    bool*   imm, bool* vec,
                    i64*    ascii_address,  char** ascii_written, bool* thread_on,
                    u64*    thread_adresse, i32 address_size,     u64* thread_rank,
                    bool*   parallel_on,    bool* parallel_off,
                    bool*   its_a_jump,     u64*  indice_label)
{
  // We'll write 32 bits => 4 bytes => 1 i32
  // 8 bits opcode
  // 9 bits "unused"
  // 5 destination
  // 5 src1
  // 5 src2
  //
  // 01 02 03 04 05 06 07 08  01 02 03 04 05 06 07 08 09  01 02 03 04 05  01 02 03 04 05  01 02 03 04 05
  // 31 30 29 28 27 26 25 24  23 22 21 20 19 18 17 16 15  14 13 12 11 10  09 08 07 06 05  04 03 02 01 00
  //
  // 2^24 ~ 2^32-1
  // opcode 0 -> 0
  // opcode 1 -> 2^24
  // opcode 2 -> 2^25
  // opcode << 24           unused will be calculated  dest << 10         src1 << 5       src2 << 0

  /*
   General idea :
    The function calculates the i32 corresponding to the instruction (see above)

   mov, jump and parallelism are handled in their own way as they require extra work

   */
  i32 position = get_op_position(tokens_list[indice][0],
                                 op_name_list, op_list_size);
  *to_write    += position << 24;

  // If it's a jump
  if (position >= 77 && position <= 83)
  {
    *its_a_jump = true;
    for (u64 k = 0; k < (u64)op_list_size;  ++k)
    {
      if (strcmp(address_array[k], tokens_list[indice][1]) == 0)
      {
        *indice_label = k;
        return;
      }
    }
    return;  // Should be unreachable
  }

  if (position == 91)
  {
    *parallel_on = true;
    u64 out[3];
    get_threads_position(tokens_list,  indice,      op_name_list,
                         op_list_size, thread_rank, out);
    immediate_value[0] = (i64)out[0];
    immediate_value[1] = (i64)out[1];
    immediate_value[2] = (i64)out[2];
    return;
  }

  else if (position == 92)
  {
    *parallel_off = true;
    return;
  }

  else if (position == 93)
  {
    *thread_on = true;
    *imm       = true;
    // We give indices from 0 to 1
    immediate_value[0] =  (i64)*thread_adresse;
    *thread_adresse    += (u64)get_number_of_one(tokens_list[indice][1]) - 1;
    immediate_value[1] =  (i64)*thread_adresse;
    *thread_adresse += 1;
    return;
  }

  i32 imm_indice = 0;

  // If mov
  if (position == 12)
  {
    if (tokens_list[indice][0][0]    == 'V'
        || tokens_list[indice][0][0] == 'T'
        || tokens_list[indice][0][0] == 'G')
    {
      *to_write += 1 << 19;
    }
    for (i32 i = 1; i < tokens_sizes[indice]; ++i)
    {
      if (tokens_list[indice][i][0]    == 'S'
          || tokens_list[indice][i][0] == 'T')
      {
        // 19 - 2 * 1 = 17 ; 19 - 2 * 2 = 15
        *to_write += 1 << (19 - 2 * i);
      }
      else if (tokens_list[indice][i][0]    == 'F'
               || tokens_list[indice][i][0] == 'G')
      {
        *to_write += 2 << (19 - 2 * i);
      }
    }
  }

  // Looking for registers
  //TODO addresses
  i32 register_indice = 0;
  for (i32 i = 1; i < tokens_sizes[indice]; ++i)
  {
    ++register_indice;
    // If it's a register or an alias
    if (isalpha(tokens_list[indice][i][0]))
    {
      // If it's an alias
     i32 j = 1;
     if (its_an_alias(tokens_list[indice][i], address_array, &j, address_size))
     {
        immediate_value[0] = (i64)address_indices[j];
        *imm = true;
        --register_indice;
        continue;
     }

      // If it's a register
      char* str_tmp = calloc((u64)(tokens_sizes[indice] + 1), sizeof(char));
      if (str_tmp == NULL)
      {
        perror("calloc in code_to_number");
        return;
      }
      strncpy(str_tmp, tokens_list[indice][i]+1, (u64)tokens_sizes[indice]);
      *to_write += atoi(str_tmp) << (15 - 5 * register_indice);
      free(str_tmp);
      // fprintf(stderr, "instr: %d ; i: %d ; rind: %d\n", ((*to_write) >> 24), i, register_indice );

      for (i32 k = 0; k < reg_size; ++k)
      {
        if (tokens_list[indice][i][0] == register_list[k][0][0])
        {
          *to_write += register_indice << (19);
        }
      }
    }

    // If it's parenthesis
    else if (tokens_list[indice][i][0]    == '('
             || tokens_list[indice][i][0] == ')')
    {
      --register_indice;
      continue;
    }

    // If it's brace
    else if (tokens_list[indice][i][0]    == '{'
             || tokens_list[indice][i][0] == '}')
    {
      --register_indice;
      continue;
    }

    // Handling immediate values
    else if (isdigit(tokens_list[indice][i][0]))
    {
      if (*imm == true)
      {
        *vec = true;
      }
      else
      {
        *imm = true;
      }
      if (toupper(tokens_list[indice][0][strlen(tokens_list[indice][0]) - 2]) == 'F'
          || toupper(tokens_list[indice][0][strlen(tokens_list[indice][0]) - 2]) == 'G')
      {
        double tmp = atof(tokens_list[indice][i]);
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wstrict-aliasing"
        immediate_value[imm_indice++] = *(i64*)&tmp;
        #pragma GCC diagnostic pop
      }
      else
      {
        immediate_value[imm_indice++] = atoi(tokens_list[indice][i]);
      }
      // *to_write += 1 << (15);
      // *to_write += immediate_value[imm_indice++] << (15);
    }

    // Handling variables
    else if (tokens_list[indice][i][0] == '$')
    {
      for (i32 j = 1; j < code_start; ++j)
      {
        if (strcmp(tokens_list[j][1], tokens_list[indice][i]+1) == 0)
        {
          if (strcasecmp(tokens_list[j][0], "u64") == 0 || strcasecmp(tokens_list[j][0], "s64") == 0)
            immediate_value[imm_indice++] = atoi(tokens_list[j][2]);
          else if (strcasecmp(tokens_list[j][0], "f64") == 0)
          {
            double tmp = atof(tokens_list[j][2]);
            #pragma GCC diagnostic push
            #pragma GCC diagnostic ignored "-Wstrict-aliasing"
            immediate_value[imm_indice++] = *(i64*)&tmp;
            #pragma GCC diagnostic pop
            // fprintf(stderr, "%lf\n", tmp);
          }

          break;
        }
      }
      // *to_write += 1 << (15);
      if (*imm == true)
      {
        *vec = true;
      }
      else
      {
        *imm = true;
      }
    }

    // Handling pointers and ascii
    else if (tokens_list[indice][i][0] == '@')
    {
      // look into data array and write value
      for (u64 j = 0; ascii_written[j] != NULL; ++j)
      {
        // fprintf(stderr, "%s\n", ascii_written[j]);
        if (strcmp(ascii_written[j], tokens_list[indice][i]+1) == 0)
        {
          immediate_value[imm_indice++] = (ascii_address[i] + (i64)(header_size * sizeof(i64)));
          break;
        }
      //   if (strcmp(tokens_list[j][1], tokens_list[indice][i]+1) == 0)
      //   {
      //     fprintf(stderr, "hey3\n");
      //     immediate_value[imm_indice++] = atoi(tokens_list[j][2]);
      //     break;
      //   }
      //   if (ascii_written[i] != NULL
      //       && strcmp(ascii_written[j], tokens_list[indice][i]+1) == 0)
      //   {
      //     immediate_value[imm_indice++] = (ascii_address[i] + header_size * sizeof(i64));
      //     break;
      //   }
      }
      // *to_write += 1 << (15);
      if (*imm == true)
      {
        *vec = true;
      }
      else
      {
        *imm = true;
      }
    }
  }
  // Récupère la position de l'opcode
  //    -> Devient le début de ce qu'on écrit
  //    -> On définit les registres
  //    -> On regarde et gère si valeurs immédiates, variables etc
  //    -> On gère les bits "inutilisés"
  // -> Retour dans un buffer fourni en entrée
  return;
}


u64 assemble_code(char***   tokens_list,
                  i32 const code_start,
                  i32 const tokens_size,   i32*   tokens_sizes,
                  char***   op_name_list,  i32    op_list_size,
                  char***   register_list, i32    reg_size,
                  char**    address_array, i32*   code_to_write,
                  i64*      imm_to_write,  i64*   imm_indice,
                  i64*      ascii_address, char** ascii_written,
                  u64*      taille_code,   i32    address_size,
                  i64*      thread_array,  i64*   thread_mask, i64* thread_number,
                  u64*      thread_max,    u64    size_data)
{
  // We need to know on which line they are
  // address_array[i] <=> address_indices[i]
  u64* address_indices = calloc((u64)(address_size + 2), sizeof(u64));
  u32 j                = 0;
  u32 l                = 0;
  u64 taille           = 0;
  u64 thread_adresse   = 0;
  u64 thread_ind_addr  = 0;
  u64 thread_rank      = 0;
  u64 indice_adresse   = 0;

  // address_indices[0] = 0;
  // for (u32 i = 0; i <address_size; ++i)
  //   fprintf(stderr, "%s\n", address_array[i]);
  u64* where_to_write = calloc((u64)tokens_size, sizeof(u64));
  u64* what_to_write  = calloc((u64)tokens_size, sizeof(u64));

  for (i32 i = code_start + 1; i < tokens_size; ++i)
  {
    // print_tokens_line(code_array[i]);
    // if (its_an_alias(code_array[i], address_array))
    if (tokens_list[i][0][strlen(tokens_list[i][0]) - 1] == 58)
    {
      address_indices[j++] = htobe64(taille + size_data + header_size * sizeof(i64));
      // fprintf(stderr, "j = %d address_size = %d\n", j, address_size);
      continue;
    }

    i32  instr_to_write      = 0;
    i64* immediate_value     = calloc(8, sizeof(i64));
    bool imm                 = false;
    bool vec                 = false;
    bool thread_on           = false;
    bool parallel_on         = false;
    bool parallel_off        = false;
    bool its_a_jump          = false;
    u64  indice_label        = 0;

    code_to_number(tokens_list,     tokens_sizes,
                   code_start,      i,
                   address_array,   address_indices,
                   op_name_list,    op_list_size,
                   &instr_to_write, immediate_value,
                   register_list,   reg_size,
                   &imm, &vec,
                   ascii_address,   ascii_written, &thread_on,
                   &thread_adresse, address_size,  &thread_rank,
                   &parallel_on,    &parallel_off,
                   &its_a_jump,     &indice_label);
//    printf("instr: %s ; imm: %d ; vec: %d\n", tokens_list[i][0], imm, vec);
    // fprintf(stderr, "instruction : %d immediate_values =", instr_to_write >> 24);

    // fprintf(stderr, "imm=%ld i=%ld l=%d\n", imm_indice[l], i, l);

    if (its_a_jump == true)
    {
      // fprintf(stderr, "%ld\n", indice_label);
      where_to_write[indice_adresse] = l;
      what_to_write[indice_adresse]  = indice_label;
      imm_indice[l]                  = (i64)*taille_code;
      ++l;
      ++indice_adresse;
      taille += sizeof(i64);
    }

    if (thread_on == true)
    {
      // fprintf(stderr, "thread_number = %ld", *thread_number);
      i64 tmp_number           = *thread_number;
      *thread_number           = tmp_number;
      thread_array[tmp_number] = (i64)htobe64(*taille_code);
      thread_mask[tmp_number]  = (i64)htobe64(retreive_value(tokens_list[i][1]));
      u64 max                  = get_max(tokens_list[i][1]);
      // fprintf(stderr, " ; max = %ld\n", max);
      if (max > *thread_max)
      {
        *thread_max = max;
      }
      free(immediate_value);
      // fprintf(stderr, "\n");
      continue;
    }

    // Handling parallel_on
    if (parallel_on == true)
    {
      for (u64 k = 0; k < 3; ++k)
      {
        imm_to_write[l] = (i64)htobe64((u64)immediate_value[k]);
        // fprintf(stderr, " %ld", immediate_value[k]);
        imm_indice[l]   = (i64)*taille_code;
        thread_ind_addr = l;
        ++l;
        taille += sizeof(i64);  // Immediate value is 64 bits -> 8 bytes
        // taille += sizeof(i64);
      }
    }
    // Handling parallel_off
    if (parallel_off == true)
    {
      imm_to_write[thread_ind_addr] = (i64)*taille_code;
      // fprintf(stderr, " %ld", *taille_code);
      free(immediate_value);
      // fprintf(stderr, "\n");
      continue;
    }

    // Checking how many immediate we have to write
    if (imm == true)
    {
      u8 max = 8;
      if (vec == false)
      {
        max = 1;
      }
      if (thread_on == true)
      {
        max = 2;
      }
      // Storing immediates to write
      for (u64 k = 0; k < max; ++k)
      {
        imm_to_write[l] = (i64)htobe64((u64)immediate_value[k]);
        // fprintf(stderr, "l=%d imm=%ld\n", l, immediate_value[k]);
        imm_indice[l]   = (i64)*taille_code;
        ++l;
        taille += sizeof(i64);  // Immediate value is 64 bits -> 8 bytes
        // taille += sizeof(i64);
      }
    }
    code_to_write[*taille_code] = (i32)htobe32((u32)instr_to_write);
    *taille_code                = *taille_code + 1;
    taille                      += sizeof(i32);
    // 1 i32 = 4 bytes
    // taille += sizeof(i32);

    free(immediate_value);
    // fprintf(stderr, "\n");
  }

  for (u64 i = 0; i < indice_adresse; ++i)
  {
    imm_to_write[where_to_write[i]] = (i64)address_indices[what_to_write[i]];
    // fprintf(stderr, "%ld\n", what_to_write[i]);
  }

  free(address_indices);
  free(where_to_write);
  free(what_to_write);
  return taille;
}


u64 assemble_data(char*** tokens_list,   i32 const code_start,
                  char**  data_to_write, i64*      nb_data_to_write,
                  i64*    ascii_address, char**    ascii_written)
{
  i32 indice = 0;
  u64 retour = 0;
  for (i32 i = 0; i < code_start; ++i)
  {
    if (strcmp(tokens_list[i][0], "ascii") == 0)
    {
      // taille is lenght of string including white space
      u64 taille  = strlen(tokens_list[i][2]) + 1;
      u64 taille2 = strlen(tokens_list[i][1]) + 1;

      data_to_write[indice] = malloc(taille  * sizeof(char));
      ascii_written[indice] = malloc(taille2 * sizeof(char));
      ascii_address[indice] = (i64)retour;

      memcpy(data_to_write[indice],   tokens_list[i][2], taille);
      memcpy(ascii_written[indice++], tokens_list[i][1], taille2);  // Retaining who we wrote
      retour += taille;
      *nb_data_to_write += 1;
    }
  }
  return retour;
}


void write_header(FILE* fp, u64 size_data, u64 size_code, i64 thread_number, u64 max_thread)
{
  i64 magic_number    = 0x4152434859302E30;
  i64 size_header     = header_size * sizeof(i64);
  i64 address_data    = size_header;
  i64 address_code    = address_data + (i64)size_data;
  i64 address_threads = address_code + (i64)size_code;
  i64 size_threads    = thread_number * 2 * (i64)sizeof(i64);
  i64 size_total      = address_threads + size_threads;
  i64 to_write[header_size] = {(i64)htobe64((u64)magic_number),
                               (i64)htobe64((u64)size_header),
                               (i64)htobe64((u64)address_data),
                               (i64)htobe64((u64)size_data),
                               (i64)htobe64((u64)address_code),
                               (i64)htobe64((u64)size_code),
                               (i64)htobe64((u64)address_threads),(i64)htobe64((u64)size_threads),
                               (i64)htobe64((u64)max_thread),
                               (i64)htobe64((u64)size_total)};
  if (fwrite(to_write, sizeof(i64), header_size, fp) == 0)
    {
      fprintf(stderr, "An error happened upon writing, exiting.\n");
      //TODO complete error handling
    }
  // for (i64 i = 0; i <header_size; ++i)
  // {
  //   fprintf(stderr, "%ld\n", be64toh(to_write[i]));
  // }
  return;
}


void write_data(FILE* fp, char** data_to_write, i64* nb_data_to_write)
{
  for (i32 i = 0; i < *nb_data_to_write; ++i)
  {
   // fprintf(stderr, "data: '%s'\n", data_to_write[i]);
    fwrite(data_to_write[i], sizeof(char), strlen(data_to_write[i]) + 1, fp);
  }
  return;
}


void write_code(FILE* fp,           i32* code_to_write,
                i64*  imm_to_write, i64* imm_indice, u64 k)
{
  i32 j = 0;
  for (u64 i = 0; i < k; ++i)
  {
    // fprintf(stderr, "instr: %d", be32toh(code_to_write[i]) >> 24);
    fwrite(code_to_write+i, sizeof(i32), 1, fp);

    // Write immediate values (e.g. movui)
    while (i == (u64)imm_indice[j])
    {
      // fprintf(stderr, " imm: %ld", imm_indice[j]);
      fwrite(imm_to_write+j, sizeof(i64), 1, fp);
      ++j;
    }
    // fprintf(stderr, "\n");
  }
  return;
}


void write_threads(FILE* fp, i64* thread_array, i64* thread_masks, i64 thread_number)
{
  for (i64 i = 0; i <= thread_number; ++i)
  {
    // fprintf(stderr, "thread: %ld %ld\n", be64toh(thread_array[i]), be64toh(thread_masks[i]));
    fwrite(thread_array+i, sizeof(i64), 1, fp);
    fwrite(thread_masks+i, sizeof(i64), 1, fp);
  }
  return;
}


void write_stuff(char*     path,
                 char***   tokens_list,   i32* tokens_list_sizes, i32 nb_tokens,
                 i32 const data_start,    i32 const code_start,
                 char***   op_name_list,  i32 op_size,
                 char***   register_list, i32 register_size,
                 char**    address_array, i32  address_size)
{
  /*
   NOTE
   Pour les ascii :
    j'écris et je garde en mémoire (dans une liste à ajouter) où je suis
      on conserve l'endroit par rapport au début de la section

    Pour les adresses d'ascii : on utilise le truc au dessus

    Pour les alias :
      Si on en croise sans opcode -> garder où il est (indice)
        Quand on doit l'écrire -> garder son emplacement (ajouter liste pour ça)
      Si on en croise avec opcode -> garder indice où il est
        Écrire son adresse (liste précédemment créée)
   */

  FILE*       file;
  struct stat st;

  if (stat(path, &st) == -1)
  {
    if (errno == 2)
    {
      goto CREATE_FILE;
    }
    perror("stat in write_stuff");
    return;
  }
  if (!S_ISREG(st.st_mode))
  {
    fprintf(stderr, "%s is a directory\n", path);
    return;
  }

  CREATE_FILE:
  file = fopen(path, "w");
  if(file == NULL)
  {
    fprintf(stderr, "Could not open %s\n", path);
    perror("fopen in write_stuff");
    return;
  }

  // Size will be big for the sake of simplicity
  char** data_to_write = calloc((u64)code_start,   sizeof(char*));
  char** ascii_written = calloc((u64)code_start,   sizeof(char*));
  i64*   ascii_address = calloc((u64)(nb_tokens - code_start), sizeof(i64));
  i32*   code_to_write = calloc((u64)(nb_tokens - data_start), sizeof(i32));
  i64*   imm_to_write  = calloc((u64)(nb_tokens - data_start), sizeof(i64));
  i64*   imm_indice    = calloc((u64)(nb_tokens - data_start), sizeof(i64));
  i64*   thread_array  = calloc((u64)(nb_tokens - code_start), sizeof(i64));
  i64*   thread_masks  = calloc((u64)(nb_tokens - code_start), sizeof(i64));
  i64*   thread_on_addresses = calloc((u64)(nb_tokens - data_start), sizeof(i64));
  i64*   thread_on_indices   = calloc((u64)(nb_tokens - data_start), sizeof(i64));
  i64    thread_number = 0;
  u64    thread_max    = 1;

  i64 nb_data_to_write            = 0;
  u64 taille_code                 = 0;
  for (i64 indice = 0; indice < nb_tokens - data_start; ++indice)
    imm_indice[indice] = -1;

  u64 size_data = assemble_data(tokens_list, code_start, data_to_write,
                                &nb_data_to_write,
                                ascii_address, ascii_written);

  u64 size_code = assemble_code(tokens_list,   code_start,
                                nb_tokens,     tokens_list_sizes,
                                op_name_list,  op_size,
                                register_list, register_size,
                                address_array, code_to_write,
                                imm_to_write,  imm_indice,
                                ascii_address, ascii_written,
                                &taille_code,  address_size,
                                thread_array,  thread_masks, &thread_number,
                                &thread_max,   size_data);
  free_char2(ascii_written, code_start);
  free(thread_on_addresses);
  free(thread_on_indices);
  free(ascii_address);

  write_header (file, size_data,     size_code,    thread_number, thread_max);
  write_data   (file, data_to_write, &nb_data_to_write);
  write_code   (file, code_to_write,
                imm_to_write, imm_indice, taille_code);
  // write_threads(file, thread_array,  thread_masks, thread_number);

  free_char2(data_to_write, code_start);
  free(code_to_write);
  free(imm_to_write);
  free(thread_array);
  free(thread_masks);
  free(imm_indice);

  (void)fclose(file);

  // printf("Success\n");
  return;
}
