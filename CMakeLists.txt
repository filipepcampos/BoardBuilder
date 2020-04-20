cmake_minimum_required(VERSION 3.16)
project(TestBoardBuilder)

set(CMAKE_CXX_STANDARD 14)

add_executable(TestBoardBuilder main.cpp Board.h Board.cpp InputHandler.h InputHandler.cpp)