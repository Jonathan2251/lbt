#!/bin/bash -a

prog=gcc-loops

LLVM_INSTALL=~/test/polly/llvm_arm_build
pollycc="${LLVM_INSTALL}/bin/clang++ -Xclang -load -Xclang ${LLVM_INSTALL}/lib/LLVMPolly.so"

# order of options do matters, ${pollycc} -fno-vectorize -O3: meaning O3 enable vectorize
${pollycc} -O3 -fno-vectorize -mcpu=cortex-a9 -mfloat-abi=hard -mfpu=neon ${prog}.cpp -o ${prog}-novec
${pollycc} -O3 -fno-vectorize -mllvm -polly -mllvm -polly-ignore-aliasing -mcpu=cortex-a9 -mfloat-abi=hard -mfpu=neon ${prog}.cpp -o ${prog}-polly
${pollycc} -O3 -fno-vectorize -mllvm -polly -mllvm -polly-vectorizer=polly -mllvm -polly-ignore-aliasing -mcpu=cortex-a9 -mfloat-abi=hard -mfpu=neon ${prog}.cpp -o ${prog}-pollyvec
${pollycc} -O3 -mllvm -polly -mllvm -polly-ignore-aliasing -mcpu=cortex-a9 -mfloat-abi=hard -mfpu=neon ${prog}.cpp -o ${prog}-clangvec_polly
${pollycc} -O3 -mcpu=cortex-a9 -mfloat-abi=hard -mfpu=neon ${prog}.cpp -o ${prog}-clangvec

scp run.sh ${prog}-novec ${prog}-polly ${prog}-pollyvec ${prog}-clangvec_polly ${prog}-clangvec root@10.19.132.196:/root/cschen/

