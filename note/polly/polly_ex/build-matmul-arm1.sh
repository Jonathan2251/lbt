#!/bin/sh -a

rm matmul matmul1
LLVM_INSTALL=~/test/polly/llvm_arm_build

${LLVM_INSTALL}/bin/clang -O3 -DTEST -mcpu=cortex-a9 -mfloat-abi=hard -mfpu=neon matmul.c -o pmatmul
${LLVM_INSTALL}/bin/clang -O3 -DTEST -mcpu=cortex-a9 -mfloat-abi=hard -mfpu=neon matmul1.cpp -o pmatmul1

${LLVM_INSTALL}/bin/clang -O3 -mcpu=cortex-a9 -mfloat-abi=hard -mfpu=neon matmul.c -o matmul
${LLVM_INSTALL}/bin/clang -O3 -mcpu=cortex-a9 -mfloat-abi=hard -mfpu=neon matmul1.cpp -o matmul1

scp pmatmul pmatmul1 matmul matmul1 root@10.19.132.196:/root/cschen/
#scp matmul matmul1 root@10.19.132.196:/root/cschen/


