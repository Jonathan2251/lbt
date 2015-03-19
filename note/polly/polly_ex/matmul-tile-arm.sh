#!/bin/sh -a

rm matmul-notile matmul-tile matmul-notile.result matmul-tile.result
LLVM_INSTALL=~/test/polly/llvm_arm_build
alias pollycc="${LLVM_INSTALL}/bin/clang -Xclang -load -Xclang ${LLVM_INSTALL}/lib/LLVMPolly.so"
alias opt="opt -load ${LLVM_INSTALL}/lib/LLVMPolly.so"

${LLVM_INSTALL}/bin/clang -O3 -mcpu=cortex-a9 -mfloat-abi=hard -mfpu=neon matmul-notile.cpp -o matmul-notile
${LLVM_INSTALL}/bin/clang -O3 -mcpu=cortex-a9 -mfloat-abi=hard -mfpu=neon matmul-tile.cpp -o matmul-tile

echo "\n\nNo print result"
echo "time ./matmul"
time -f "%E real, %U user, %S sys" ./matmul-notile
echo "time ./matmul-tile"
time -f "%E real, %U user, %S sys" ./matmul-tile

scp matmul-notile matmul-tile root@10.19.132.196:/root/cschen/


