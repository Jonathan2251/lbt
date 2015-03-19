#!/bin/sh -a

rm fusion fusion.polly
LLVM_INSTALL=~/test/polly/llvm_build
alias pollycc="${LLVM_INSTALL}/bin/clang -Xclang -load -Xclang ${LLVM_INSTALL}/lib/LLVMPolly.so"
alias opt="opt -load ${LLVM_INSTALL}/lib/LLVMPolly.so"

${LLVM_INSTALL}/bin/clang -O3 -S fusion.c -emit-llvm -o fusion.ll
pollycc -O3 -S fusion.c -emit-llvm -o fusion.no-polly.ll
pollycc -mllvm -polly -O3 -S fusion.c -emit-llvm -o fusion.polly.ll
echo "--> 3. Prepare the LLVM-IR for Polly"
opt -S -mem2reg -loop-simplify -polly-indvars fusion.no-polly.ll > fusion.preopt.ll
echo "--> 4. Show the SCoPs detected by Polly"
opt -basicaa -polly-cloog -analyze -q fusion.preopt.ll
echo "--> 3. Prepare the LLVM-IR for Polly"
opt -S -mem2reg -loop-simplify -polly-indvars fusion.polly.ll > fusion.preopt.ll
echo "--> 4. Show the SCoPs detected by Polly"
opt -basicaa -polly-cloog -analyze -q fusion.preopt.ll

${LLVM_INSTALL}/bin/clang -O3 -DTEST fusion.c -o fusion
${LLVM_INSTALL}/bin/clang -O3 -DTEST fusion_handcode.c -o fusion_handcode
#pollycc -mllvm -polly -O3 -DTEST fusion.c -o fusion.polly

echo "\nWith print result"
echo "time ./fusion"
time -f "%E real, %U user, %S sys" ./fusion > fusion.result
echo "time ./fusion_handcode"
time -f "%E real, %U user, %S sys" ./fusion_handcode > fusion_handcode.result
#echo "time ./fusion.polly"
#time -f "%E real, %U user, %S sys" ./fusion.polly > fusion.polly.result

${LLVM_INSTALL}/bin/clang -O3 fusion.c -o fusion
pollycc -mllvm -polly -mllvm -polly-ignore-aliasing -mllvm -polly-opt-fusion=max -mllvm -polly-no-tiling -O3 fusion.c -o fusion.polly
${LLVM_INSTALL}/bin/clang -O3 fusion_handcode.c -o fusion_handcode
gcc -O3 fusion.c -o fusion.gcc
#pollycc -mllvm -polly -O3 fusion.c -o fusion.polly

echo "\n\nNo print result"
echo "time ./fusion"
time -f "%E real, %U user, %S sys" ./fusion
echo "time ./fusion_handcode"
time -f "%E real, %U user, %S sys" ./fusion_handcode
echo "time ./fusion.gcc"
time -f "%E real, %U user, %S sys" ./fusion.gcc
#echo "time ./fusion.polly"
#time -f "%E real, %U user, %S sys" ./fusion.polly

echo "do diff fusion.result fusion.polly.result to ensure the output is same"
diff fusion.result fusion_handcode.result


