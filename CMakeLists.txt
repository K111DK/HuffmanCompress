cmake_minimum_required(VERSION 3.20)
project(HuffmanCompress C)

set(CMAKE_C_STANDARD 99)
add_executable(HuffmanCompress main.c BasicStruct.h utils.h  Compress.h DeCompress.h    )
MATH(EXPR stack_size "4*1024*1024")
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--stack,${stack_size}")