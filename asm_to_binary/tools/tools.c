#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/stat.h>

// #include "../parse/parse.h"
#include "tools.h"
#include "types.h"


//
void remove_space(char const *str_in, char *str_out)
{
  // Allocating memory and initialiazin looping elements
  i32 taille_in  = (sizeof(char) * strlen(str_in))  + sizeof(char);
  i32 taille_out = (sizeof(char) * strlen(str_out)) + sizeof(char);
  char* str_tmp  = calloc(taille_out, sizeof(char));

  i32 i = 0;
  i32 j = 0;

  i32 quote_flag = false;

  while (i < taille_in)
  {
    if(str_in[i] == '\"')
    {
      if(quote_flag)
      {
        quote_flag = false;
      }
      else
      {
        quote_flag = true;
      }
    }

    // 32 is space's ASCII code, 9 is end of line
    if((str_in[i] != ' ' && str_in[i] != 9 && str_in[i] != 10)
       || quote_flag)
    {
      str_out[j] = str_in[i];
      ++j;
    }
    ++i;
  }
  free(str_tmp);
}



/// @brief Function used to verify if argc >= 2 (i.e. their is a filename in argv[1]) and to test if the file does exist.
/// @param argc Number of args passed in command line
/// @param argv Values of args
/// @return True if everything is correct, false if argv < 2 OR argv[1] file does not exist
bool file_verification(i32 const argc, char** const argv)
{
  //Is their a file name as argument ?
  if(argc < 2)
  {
    fprintf(stderr,"USAGE : ./asm [FILENAME]\n");
    return false;
  }

  //Does the file in argv[1] exist ?
  if(access(argv[1],F_OK) != 0)
  {
    fprintf(stderr,"Error : the file %s does not exist.\n", argv[1]);
    return false;
  }

  return true;
}



/// @brief Function used to load in memory a file. This is practical performance wise.
/// @param filename Name of the file to load in memory
/// @return A string containing the whole file
char* load_file(char* const filename)
{
  i32   file_size; //Size of the file, used for malloc
  FILE* fileptr = fopen(filename, "r"); //Pointer of the file

  //We go to the end of the file
  fseek(fileptr, 0L, SEEK_END);

  //Now we retreive where we are (eq. the size of the file)
  file_size = ftell(fileptr);

  //Now we just copy the whole file into a buffer
  char* buffer = (char*)malloc(sizeof(char) * (file_size + 1)); //Setting up the buffer, +1 for \0 just to be sure.
  rewind(fileptr); //Going back to the beginning of our stream
  fread(buffer, sizeof(char), file_size, fileptr);
  buffer[file_size + 1] = '\0';

  return buffer;
}



/// @brief Function used to print a file loaded in memory (mostly for debug purposes)
/// @param Pointer of the file loaded in memory
void print_file(char* const file)
{
  i32 i = 0;
  while(file[i] != '\0')
  {
    printf("%c", file[i]);
    i++;
  }
}



/// @brief print tokens list (print each char* until NULL is read, will also print the separators)
/// @param tokens List of tokens
/// @param separator separator used to retreive tokens, will also be printed back
void print_tokens_line(char** const tokens, i32 size)
{
  i32 i = 0;
  printf("LINE : [");
  while(i < size)
  {
    printf("'%s',",tokens[i++]);
  }
  printf("]\n");
}



void print_tokens_list(char*** const token_list, i32* sizes, i32 lenght)
{
  //When we find an empty line we stop
  i32 i = 0;
  while(i < lenght)
  {
    print_tokens_line(token_list[i], sizes[i]);
    i++;
  }
}



/// The functions generates the list of words from line, stored in out
/// It returns the number of words in out
/// out should be malloc'd with a size of at most len,
///   and return value malloc will be assigned inside of it
///
/// len argument is size of line, excluding termination null byte
///
/// If return value is -1, then an error has occured
///   If an error occurs, function will free the memory it assigned
/// (Do note that the function does not handle SIGINT)
i32 make_tokens(char* line, char** out, u64 len)
{
  // number is the return value (number of tokens in line)
  i32 number       = 0;
  // is_in_a_word == 0 -> not in word ; 1 if in a word
  i32 is_in_a_word = 0;
  // is_in_quotes == 0 -> not in quotes ; 1 if in quotes
  i32 is_in_quotes = 0;
  // indice_out is current indice in out list
  i32 indice_out   = 0;
  // current_size is size of current word
  i32 current_size = 0;

  char* tmp = calloc(++len, sizeof(char));
  if (tmp == NULL)
  {
    perror("calloc in make_tokens");
    number = -1;
    goto END_make_tokens;
  }

  for (u64 i = 0; i < len; ++i)
  {
    // Handling character chains
    if (line[i] == '"')
    {
      // If was not already in chain
      if (is_in_quotes == 0)
      {
        ++is_in_quotes;
        ++number;
        ++is_in_a_word;
        continue;
      }
      else
      {
        --is_in_quotes;
        goto WRITE_make_tokens;
      }
    }

    // Acts like a separator but is a token itself
    if ((line[i] == '(' || line[i] == ')'
        || line[i] == '{' || line[i] == '}')
          && is_in_quotes == 0)
    {
      if (is_in_a_word == 1)
      {
        // TODO: make a function to avoid code duplication
        --is_in_a_word;
        out[indice_out] = malloc(++current_size);
        if (out[indice_out] == NULL)
        {
          perror("malloc in make_tokens");
          for (i32 i = 0; i < number; ++i)
          {
            free(out[i]);
          }
          number = -1;
          goto END_make_tokens;
        }
        memcpy(out[indice_out++], tmp, current_size);
        memset(tmp, 0, len);
        current_size = 0;
        goto ELSE_PAR_make_tokens;
      }
      else
      {
        ELSE_PAR_make_tokens:
        ++is_in_a_word;
        ++number;
        tmp[current_size++] = line[i];
        WRITE_make_tokens:
        --is_in_a_word;
        out[indice_out] = malloc(++current_size);
        if (out[indice_out] == NULL)
        {
          perror("malloc in make_tokens");
          for (i32 i = 0; i < number; ++i)
          {
            free(out[i]);
          }
          number = -1;
          goto END_make_tokens;
        }
        memcpy(out[indice_out++], tmp, current_size);
        memset(tmp, 0, len);
        current_size = 0;
        continue;
      }
    }

    // Handling blanks, end of words etc
    else if ((line[i] < 33 || line[i] == ',') && is_in_quotes == 0)
    {
      if (is_in_a_word == 1)
      {
        goto WRITE_make_tokens;
      }
    }

    // Binary doesn't care about your comments
    else if (line[i] == '#' && is_in_quotes == 0)
    {
      break;
    }

    // "Regular" character
    else
    {
      // If not in a word
      if (is_in_a_word == 0)
      {
        ++number;
        ++is_in_a_word;
      }
      // Adding character to current chain
      tmp[current_size++] = line[i];
    }
  }

  END_make_tokens:
  free(tmp);
  return number;
}


/// Applies make_tokens on every line of f
/// Returns the number of lines
/// out will contain a list of make_tokens out (cf make_tokens)
/// sizes will contain a list of make_tokens return values
///
/// out and sizes must be alloc'd before calling the function,
///   for now I advise a size of f's size (requires examination)
///
/// file descriptor given will be closed at the end of the function
///
/// If return value is -1 -> a malloc couldn't be done or make_tokens returned -1 and memory has been cleaned
/// If return value is -2 -> an error occured when closing f, memory hasn't been cleaned
/// If return value is -3 -> both previous errors occured, memory has been cleaned
/// (Do note that the function does not handle SIGINT)
i32 tokenize(FILE* f, char*** out, i32* sizes)
{
  u64   len;
  u64   buffer_size;
  char* line;
  u32   i;

  line         = NULL;
  buffer_size  = 0;

  // indice is current indice in out and return value
  i32 indice = 0;
  // token_line is return value
  i32 make_tokens_return_value = 0;

  // Iterating on every line of the file
  len = getline(&line, &buffer_size, f);
  for(i = 1; len != -1; ++i)
  {
    // Allocating memory
    out[indice] = malloc(buffer_size * sizeof(char*));
    if (out[indice] == NULL)
    {
      // If memory could not be assigned, all memory is freed
      perror("malloc in tokenize");

      // TODO consider this comment
      // Might go there if malloc returns an error
      //    ~> no memory left, but we create variables
      //    => use already existing one?
      CLEAN_tokenize:
      for (u64 k = 0; k < indice - 1; ++k)
      {
        for (u64 l = 0; l < sizes[k]; ++l)
        {
          free(out[k][l]);
        }
        free(out[k]);
      }
      free(out[indice]);
      indice = -1;
      goto END_tokenize;
    }

    // Creating tokens
    make_tokens_return_value = make_tokens(line, out[indice], len);

    // If an error occured while creating tokens
    //    free memory and return error
    if (make_tokens_return_value == -1)
    {
      goto CLEAN_tokenize;
    }

    // If line was empty
    if (make_tokens_return_value == 0)
    {
      // Free malloc (will realloc it after)
      free(out[indice]);
      // Calculate len of next line and skip to next iteration
      len = getline(&line, &buffer_size, f);
      continue;
    }

    // Store number of tokens and go to next iteration
    sizes[indice] = make_tokens_return_value;
    len           = getline(&line, &buffer_size, f);
    ++indice;
  }

  // Closing file, freeing non-returned memory and returning result
  END_tokenize:
  if (fclose(f) != 0)
  {
    perror("fclose in tokenize");
    if (indice == -1)
    {
      indice = -3;
    }
    else
    {
      indice = -2;
    }
  }
  free(line);
  return indice;
}


void free_char2(char** char2, i32 size_dim_1)
{
  //If already empty
  if(char2 == NULL)
  {
    return;
  }

  for(i32 i = 0; i < size_dim_1; i++)
  {
    if(char2[i] != NULL)
    {
      free(char2[i]);
    }
  }
  free(char2);
}


void free_char3(char*** char3, i32 size_dim_1, i32 size_dim_2)
{
  //If already empty
  if(char3 == NULL)
  {
    return;
  }

  for(i32 i = 0; i < size_dim_1; ++i)
  {
    if(char3[i] != NULL)
    {
      for(i32 j = 0; j < size_dim_2; ++j)
      {
        if(char3[i][j] != NULL)
        {
          free(char3[i][j]);
        }
      }
      free(char3[i]);
    }
  }
  free(char3);
}
