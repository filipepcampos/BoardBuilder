cmake_minimum_required(VERSION 3.10)
project(BoardBuilder)

set(CMAKE_CXX_STANDARD 14)

add_executable(BoardBuilder main.cpp Board.h Board.cpp IO.h IO.cpp Word.cpp Word.h)