cmake_minimum_required(VERSION 3.9)
project(CustomShell)

set(CMAKE_CXX_STANDARD 11)

add_executable(CustomShell main.cpp shell.cpp shell.h command.cpp command.h token-old.cpp token-old.h)