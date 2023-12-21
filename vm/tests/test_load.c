#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "load.h"

// Test the loadu function with a valid address
static void test_loadu_valid_address(void **state) {
    // Initialize CPU structure
    CPU cpu;
    cpu.U[0] = 10;
    cpu.U[1] = 20;

    // Set up the instruction: source 1 = U0, source 2 = U1, destination = U2, offset = 54
    Instruction instr;
    instr.destination = 2;
    instr.source_2 = 1;
    instr.unused = 54; 
    instr.source_1 = 0;

    // Set a value in memory at the computed address: U[0] + U[1] + instr.unused = 10 + 20 + 54 = 84
    cpu.Memory[cpu.U[0] + cpu.U[1] + instr.unused] = 50;

    // Execute the loadu function
    loadu(&cpu, instr);

    // Verify the expected result after executing loadu
    // Expecting U[2] to contain the value from the memory address: U[0] + U[1] + offset
    assert_int_equal(cpu.U[2], 50);
}

static void test_loadu_memory_overflow(void **state) {
    CPU cpu;
    cpu.U[0] = UINT64_MAX; // Valeur énorme pour simuler le dépassement de mémoire
    cpu.U[1] = 20;

    Instruction instr;
    instr.destination = 2;
    instr.source_2 = 1;
    instr.unused = 0;
    instr.source_1 = 0;

    // Tentative d'accéder à une adresse mémoire au-delà de MEMORY_SIZE
    loadu(&cpu, instr);

    // Vérification si la valeur dans cpu->U[2] n'a pas été modifiée
    assert_int_equal(cpu.U[2], 0); // Vérification de la non-modification de la valeur
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_loadu_valid_address),
        cmocka_unit_test(test_loadu_memory_overflow),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}