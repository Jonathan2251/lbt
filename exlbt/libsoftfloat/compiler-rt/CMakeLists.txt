cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER clang)
set(CC clang)
set(INCFLAG "-I../include -I../common")
set(CFLAGS " -target mips-unknown-linux-gnu -emit-llvm ")
set(CMAKE_C_FLAGS_DEBUG "-target mips-unknown-linux-gnu -emit-llvm -I../include -I../common -c")
set(CMAKE_C_FLAGS "-target mips-unknown-linux-gnu -emit-llvm -I/home/Gamma/test/avr-libc-1.8.1/compiler-rt/builtins -I../include -I../common -c")
set(COMPILE_FLAGS "-target mips-unknown-linux-gnu -emit-llvm -I/home/Gamma/test/avr-libc-1.8.1/compiler-rt/builtins -I../include -I../common -c")
include_directories(../include ../common -I)

project(Compiler-rt)
add_custom_target(compiler-rt)
# First, add the subdirectories which contain feature-based runtime libraries
# and several convenience helper libraries.
add_subdirectory(builtins)

