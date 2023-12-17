cmake_minimum_required(VERSION 3.10)

project(Emulateur)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -g")

set(SRCS main.c)
set(TARGET main)

add_executable(${TARGET} ${SRCS})

add_subdirectory(operation)
target_link_libraries(${TARGET} operation)


