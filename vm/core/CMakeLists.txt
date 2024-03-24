cmake_minimum_required(VERSION 3.10)
project(CoreLibrary)

# Liste des fichiers source pour la bibliothèque operation
set(HEADER_FILES
    cpu.h
    type.h
)

set(SOURCES
    cpu.c
)


add_library(core STATIC ${SOURCES})

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})