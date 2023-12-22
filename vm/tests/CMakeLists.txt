cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -g")

project(TestEmulateur)
######################### test cmocka ############################
 
# Inclusion de la librairie Operation + cmocka
add_subdirectory(../operation operation)
include(FindCMocka.cmake)

add_executable(TestLoad test_load.c)
add_executable(TestMove test_move.c)
add_executable(TestStore test_store.c)
add_executable(TestSignedArithmLogic test_s_arithmetic_logic.c)
add_executable(TestUnsignedArithmLogic test_u_arithmetic_logic.c)
add_executable(TestConversion test_conversion.c)

# Lier les fichiers objets avec les tests
target_link_libraries(TestLoad PRIVATE operation)
target_link_libraries(TestMove PRIVATE operation)
target_link_libraries(TestStore PRIVATE operation)
target_link_libraries(TestSignedArithmLogic PRIVATE operation)
target_link_libraries(TestUnsignedArithmLogic PRIVATE operation)
target_link_libraries(TestConversion PRIVATE operation)

foreach(TEST_EXECUTABLE TestLoad TestMove TestStore TestSignedArithmLogic TestUnsignedArithmLogic TestConversion)
    target_compile_features(${TEST_EXECUTABLE} PRIVATE c_std_99)
    target_link_libraries(${TEST_EXECUTABLE} PRIVATE cmocka-static)
    add_test(NAME ${TEST_EXECUTABLE} COMMAND ${TEST_EXECUTABLE})
endforeach()

enable_testing()