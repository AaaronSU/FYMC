#include <cpu.h>
#include <stdlib.h>
#include <stdio.h>

CPU *core_create(u8 type)
{
    CPU *core = malloc(sizeof(CPU));
    if (core == NULL)
    {
        fprintf(stderr, "Memory allocation failed for core\n");
        return NULL;
    }

    // core->ID = 0; // Set ID as needed
    core->type = type;
    core->CF = 0;
    core->IP = 0;
    core->Memory = NULL;    
    core->BinaryFile = NULL;

    // Initialize registers to zero
    for (int i = 0; i < NUMBER_SCALAR_REGISTER; i++)
    {
        core->U[i] = 0;
        core->S[i] = 0;
        core->F[i] = 0.0;
    }
    
    for (int i = 0; i < NUMBER_VECTOR_REGISTER; i++)
    {
        for (int j = 0; j < SIZE_VECTOR; j++)
        {
            core->V[i][j] = 0;
            core->T[i][j] = 0;
            core->G[i][j] = 0.0;
        }
    }

    return core;
}

numa_node_t *numa_node_create(u16 number_of_core)
{
    numa_node_t *node = malloc(sizeof(numa_node_t));
    if (node == NULL)
    {
        fprintf(stderr, "Memory allocation failed for NUMA node\n");
        return NULL;
    }

    node->ID = 0;
    node->nb_cores = number_of_core;
    node->cores = malloc(number_of_core * sizeof(CPU));
    if (node->cores == NULL)
    {
        fprintf(stderr, "Memory allocation failed for cores in NUMA node\n");
        free(node);
        return NULL;
    }

    // Create management core
    CPU *management_core = core_create(1);
    if (management_core == NULL)
    {
        fprintf(stderr, "Failed to create management core\n");
        free(node->cores);
        free(node);
        return NULL;
    }
    management_core->ID = 0;
    node->cores[0] = *management_core;
    free(management_core); // Nécessaire ????

    // Create compute cores
    for (int i = 1; i < number_of_core; i++)
    {
        CPU *core = core_create(0);
        core->ID = i;
        if (core == NULL)
        {
            fprintf(stderr, "Failed to create core %d in NUMA node\n", i);
            for (int j = 1; j < i; j++)
            {
                free(&node->cores[j]);
            }
            free(node->cores);
            free(node);
            return NULL;
        }
        node->cores[i] = *core;
        free(core); // Nécessaire ?
    }

    return node;
}
