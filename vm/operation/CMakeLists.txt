cmake_minimum_required(VERSION 3.10)
project(OperationLibrary)

# Liste des fichiers source pour la bibliothèque operation
set(HEADER_FILES
    include/load.h
    include/move.h
    include/store.h
    include/conversion.h
    include/u_arithmetic_logic.h
    include/f_arithmetic_logic.h
    include/s_arithmetic_logic.h
    include/tools.h
    include/output.h
    include/lecture.h
    include/compare_jump.h
)

set(SOURCES
    src/load.c
    src/move.c
    src/store.c
    src/conversion.c
    src/u_arithmetic_logic.c
    src/f_arithmetic_logic.c
    src/s_arithmetic_logic.c
    src/tools.c
    src/output.c
    src/lecture.c
    src/compare_jump.c
)

add_subdirectory(../core core)
add_library(operation STATIC ${SOURCES} core)
target_link_libraries(operation m)

target_include_directories(operation PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(operation PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../core)
