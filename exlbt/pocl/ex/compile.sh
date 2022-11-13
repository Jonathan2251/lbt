#!/usr/bin/env bash

FILE=test_clCreateKernelsInProgram
#FILE=saxpy

/usr/lib/llvm-13/bin/clang -DCL_HPP_TARGET_OPENCL_VERSION=300 -DCL_TARGET_OPENCL_VERSION=300 -DCL_USE_DEPRECATED_OPENCL_1_0_APIS -DCL_USE_DEPRECATED_OPENCL_1_1_APIS -DCL_USE_DEPRECATED_OPENCL_1_2_APIS -DCL_USE_DEPRECATED_OPENCL_2_0_APIS -DCL_USE_DEPRECATED_OPENCL_2_1_APIS -DCL_USE_DEPRECATED_OPENCL_2_2_APIS -I$HOME/git/pocl/build -I$HOME/git/pocl/include -I$HOME/git/pocl/include/hpp -I$HOME/git/pocl/poclu -I$HOME/git/pocl -g -fPIE -Werror=implicit-function-declaration -Wincompatible-pointer-types -Wno-ignored-attributes -fvisibility=hidden -pthread -MD -MT saxpy.c.o -MF $FILE.c.o.d -o $FILE.c.o -c $FILE.c

/usr/lib/llvm-13/bin/clang -g  -pie $FILE.c.o -o a.out  -Wl,-rpath,/usr/local/lib ~/git/pocl/build/poclu/libpoclu.a ~/git/pocl/build/lib/CL/libOpenCL.so.2.9.0 -lhwloc /usr/lib/llvm-13/lib/libclang-cpp.so /usr/lib/llvm-13/lib/libLLVM-13.so -lrt -lm -ldl -lm -pthread -ldl 
