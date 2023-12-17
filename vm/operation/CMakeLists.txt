cmake_minimum_required(VERSION 3.10)
project(OperationLibrary)

# Liste des fichiers source pour la bibliothèque operation
set(HEADER_FILES
    include/load.h
    include/move.h
    include/store.h
    include/conversion.h
    include/u_arithmetic_logic.h
    include/s_arithmetic_logic.h
    include/cpu.h
    include/tools.h
    include/type.h

)

set(SOURCE_FILES
    src/load.c
    src/move.c
    src/store.c
    src/conversion.c
    src/u_arithmetic_logic.c
    src/s_arithmetic_logic.c
    src/tools.c
)


add_library(operation STATIC ${SOURCE_FILES})

target_include_directories(operation PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
