cmake_minimum_required(VERSION 3.13)
project(public_bus_system_simulator C)

set(CMAKE_C_STANDARD 99)

add_executable(public_bus_system_simulator public_bus_system_simulator.c bus_system.h pointer_functions.c file_functions.c)