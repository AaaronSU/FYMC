cmake_minimum_required(VERSION 3.10)

project(VotreProjet C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -g")

set(SRCS main.c registre.h opcode.h type.h exec_ins.h)
set(TARGET main)

add_executable(${TARGET} ${SRCS})
target_link_libraries(${TARGET} m)
