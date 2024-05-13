#define _GNU_SOURCE
#include <sched.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <stdlib.h>

#include "../vm.h"
#include "../instruction.h"

void par_on(core_t *core)
{

    // fprintf(stderr, "address %ld ", core->IP);
    instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
    core->IP += SIZE_INSTRUCTION_IN_BYTE;


    u64 start = get_immediate(core);
    u64 end   = get_immediate(core);
    u64 jump  = get_immediate(core);

    // fprintf(stderr, "%ld %ld %ld\n", start, end, jump);


    header_t header = *(header_t *)(core->file_buffer);
    u64 address_parallel_on = htobe64(header.address_parallel_on);
    u64 max_cores = htobe64(header.core_number);
    core->IP = address_parallel_on + SIZE_IMMEDIATE_IN_BYTE * (start) * 2;
    program_thread_data_t *tds = (program_thread_data_t *)malloc(sizeof(program_thread_data_t) * max_cores);

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);

    to_send_registers_t* registres = malloc(sizeof(to_send_registers_t));

    // TODO don't copy everything because it's slow
    for (int i = 0; i < NUMBER_SCALAR_REGISTER; i++)
    {
        registres->U[i] = core->U[i];
        registres->S[i] = core->S[i];
        registres->F[i] = core->F[i];
    }
    for (int i = 0; i < NUMBER_VECTOR_REGISTER; i++)
    {
        for (int j = 0; j < NUMBER_SCALAR_IN_VECTOR_REGISTER; j++)
        {
            registres->V[i][j] = core->V[i][j];
            registres->T[i][j] = core->T[i][j];
            registres->G[i][j] = core->G[i][j];
        }
    }
    for (int i = 0; i < 8; i++)
    {
        registres->CF[i] = core->CF[i];
    }

    u64 i = 0;
    for (u64 j = start; j < end; ++j)
    {
      u64 address = get_immediate(core);
      u64 mask    = get_immediate(core);
      for (u64 bitIndex = 0; bitIndex < max_cores; ++bitIndex)
      {
        if ((mask & (1 << bitIndex)) != 0)
        {
          CPU_SET(bitIndex + core->offset + 1, &cpuset);

          tds[i].memory_address = core->memory;
          tds[i].file_buffer    = core->file_buffer;
          tds[i].core_offset    = core->offset;
          tds[i].management     = false;
          tds[i].registers      = registres;
          tds[i].given_id       = i + 1;
          tds[i].mutex          = core->mutex;
          tds[i].IP             = address;

          pthread_create(&tds[i].tid, NULL, execute_program_thread, &tds[i]);
          pthread_setaffinity_np(tds[i].tid, sizeof(cpuset), &cpuset);
          ++i;
        }
      }
    }
    for (u16 j = 0; j < i; j++)
    {
        pthread_join(tds[j].tid, NULL);
    }

    free(registres);
    free(tds);

    core->IP = jump;
    // fprintf(stderr, "jumped to %ld\n", core->IP);
}


void get_core(core_t *core)
{
  instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
  core->U[instruction.register_1] = core->given_id;
  // fprintf(stderr, "id %d\n", core->id);
  core->IP += SIZE_INSTRUCTION_IN_BYTE;
}


void th_off(core_t *core)
{
  // fprintf(stderr, "thread_off %ld\n", core->IP);
  header_t header = *(header_t *)(core->file_buffer);
  core->IP = htobe64(header.size_total);  // Should result in seppuku
}



void lock_on(core_t *core)
{
  instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
  core->IP += SIZE_INSTRUCTION_IN_BYTE;

  u64 indice_to_take = get_immediate(core);
  // TODO prevent indice_to_take to be bigger in assembly
  if (indice_to_take > MAX_MUTEX)
    indice_to_take = MAX_MUTEX;

  pthread_mutex_lock(&core->mutex[indice_to_take]);
}


void lock_off(core_t *core)
{
  instruction_t instruction = instruction_new(*(u32 *)&(core->file_buffer[core->IP]));
  core->IP += SIZE_INSTRUCTION_IN_BYTE;

  u64 indice_to_take = get_immediate(core);
  // TODO prevent indice_to_take to be bigger in assembly
  if (indice_to_take > MAX_MUTEX)
    indice_to_take = MAX_MUTEX;

  pthread_mutex_unlock(&core->mutex[indice_to_take]);
}

