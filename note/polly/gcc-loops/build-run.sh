#!/bin/bash -a
prog=gcc-loops

LLVM_INSTALL=~/test/polly/llvm_build
pollycc="${LLVM_INSTALL}/bin/clang++ -Xclang -load -Xclang ${LLVM_INSTALL}/lib/LLVMPolly.so"

# order of options do matters, ${pollycc} -fno-vectorize -O3: meaning O3 enable vectorize
${pollycc} -O3 -fno-vectorize ${prog}.cpp -o ${prog}-novec
${pollycc} -O3 -fno-vectorize -mllvm -polly -mllvm -polly-ignore-aliasing ${prog}.cpp -o ${prog}-polly
${pollycc} -O3 -fno-vectorize -mllvm -polly -mllvm -polly-vectorizer=polly -mllvm -polly-ignore-aliasing ${prog}.cpp -o ${prog}-pollyvec
${pollycc} -O3 -mllvm -polly -mllvm -polly-ignore-aliasing ${prog}.cpp -o ${prog}-clangvec_polly
${pollycc} -O3 ${prog}.cpp -o ${prog}-clangvec

echo "time ./${prog}-novec"
sudo time ./${prog}-novec 1
echo "time ./${prog}-polly"
sudo time ./${prog}-polly 1
echo "time ./${prog}-pollyvec"
sudo time ./${prog}-pollyvec 1
echo "time ./${prog}-clangvec_polly"
sudo time ./${prog}-clangvec_polly 1
echo "time ./${prog}-clangvec"
sudo time ./${prog}-clangvec 1 

