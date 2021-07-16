cmake_minimum_required(VERSION 3.16)
project(chameleon)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(chameleon main.cpp
        src/types.h
        src/bitboard.h src/bitboard.cpp
        src/movegen.h  src/movegen.cpp
        src/position.h src/position.cpp
     )
