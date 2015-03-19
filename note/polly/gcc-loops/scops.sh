#!/bin/sh -a
prog=gcc-loops

LLVM_INSTALL=~/test/polly/llvm_build
alias pollycc="${LLVM_INSTALL}/bin/clang++ -Xclang -load -Xclang ${LLVM_INSTALL}/lib/LLVMPolly.so"
alias opt="opt -load ${LLVM_INSTALL}/lib/LLVMPolly.so"

${LLVM_INSTALL}/bin/clang -O3 -S ${prog}.cpp -emit-llvm -o ${prog}.ll
pollycc -O3 -fno-vectorize -S ${prog}.cpp -emit-llvm -o ${prog}.no-vec.ll
pollycc -O3 -S ${prog}.cpp -emit-llvm -o ${prog}.no-polly.ll
pollycc -O3 -mllvm -polly -mllvm -polly-ignore-aliasing -S ${prog}.cpp -emit-llvm -o ${prog}.polly.ll
echo "--> 3. Prepare the LLVM-IR for Polly"
opt -S -mem2reg -loop-simplify -polly-indvars ${prog}.no-vec.ll > ${prog}.preopt.ll
echo "--> 4. Show the SCoPs detected by Polly"
opt -basicaa -polly-cloog -analyze -q ${prog}.preopt.ll
echo "--> 3. Prepare the LLVM-IR for Polly"
opt -S -mem2reg -loop-simplify -polly-indvars ${prog}.no-polly.ll > ${prog}.preopt.ll
echo "--> 4. Show the SCoPs detected by Polly"
opt -basicaa -polly-cloog -analyze -q ${prog}.preopt.ll
echo "--> 3. Prepare the LLVM-IR for Polly"
opt -S -mem2reg -loop-simplify -polly-indvars ${prog}.polly.ll > ${prog}.preopt.ll
echo "--> 4. Show the SCoPs detected by Polly"
opt -basicaa -polly-cloog -analyze -q ${prog}.preopt.ll

