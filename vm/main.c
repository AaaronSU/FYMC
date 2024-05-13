#define _GNU_SOURCE
#include <sched.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <stdlib.h>
#include "vm.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Error: Incorrect number of arguments\n");
        fprintf(stderr, "Usage: %s <config>\n", argv[0]);
        return 1;
    }
    u16 n;
    char *file_buffer_list[MAX_FILE_BUFFER_SIZE];

    read_config(argv[1], file_buffer_list, &n);
    cpu_set_t cpuset;
    program_thread_data_t *tds = (program_thread_data_t *)malloc(sizeof(program_thread_data_t) * n);
    set_up_instruction_set();
    CPU_ZERO(&cpuset);

    u64 offset = 0;

    for (u16 i = 0; i < n; i++)
    {
        // TODO Check that core_number is not too big
        header_t header = *(header_t *)(file_buffer_list[i]);
        CPU_SET(i + offset, &cpuset);

        tds[i].file_buffer = file_buffer_list[i];
        tds[i].core_offset = (u32)offset;
        tds[i].management  = true;
        tds[i].given_id = 0;
        tds[i].index = i;
        offset += htobe64(header.core_number);

        pthread_create(&tds[i].tid, NULL, execute_program_thread, &tds[i]);
        pthread_setaffinity_np(tds[i].tid, sizeof(cpuset), &cpuset);
    }

    for (u16 i = 0; i < n; i++)
    {
        pthread_join(tds[i].tid, NULL);
        free(tds[i].file_buffer);
        printf("The thread %2d successful executed at core_id: %2d on numa_id: %d\n", tds[i].index, tds[i].core_id, tds[i].numa_id);
    }

    free(tds);

    return 0;
}
