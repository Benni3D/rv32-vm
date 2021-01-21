cmake_minimum_required(VERSION 3.17)
project(rv32i_emulator CXX)

set(CMAKE_CXX_STANDARD 20)
include_directories(include)
find_package(fmt)

add_executable(rv32i_emulator main.cpp include/instr.hpp include/regs.hpp include/cpu.hpp include/mem.hpp include/bus.hpp include/devices.hpp)
target_link_libraries(rv32i_emulator fmt)