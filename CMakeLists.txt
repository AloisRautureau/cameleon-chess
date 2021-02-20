cmake_minimum_required(VERSION 3.16)
project(baub_chess)

set(CMAKE_CXX_STANDARD 14)

add_executable(baub_chess main.cpp
        ChessEngine/bb.cpp ChessEngine/bb.h ChessEngine/board_representation.cpp ChessEngine/board_representation.h)
