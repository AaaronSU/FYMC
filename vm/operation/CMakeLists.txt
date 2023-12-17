cmake_minimum_required(VERSION 3.10)
project(OperationLibrary)

# Liste des fichiers source pour la bibliothèque operation
set(HEADER_FILES
    include/load.h
    include/mov.h
    include/store.h
    include/cpu.h
    include/tools.h
    include/type.h
)

set(SOURCE_FILES
    src/load.c
    src/mov.c
    src/store.c
    src/tools.c
)


add_library(operation STATIC ${SOURCE_FILES})

target_include_directories(operation PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
