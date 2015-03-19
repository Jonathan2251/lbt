#!/bin/sh -a

rm matmul-notile matmul-tile matmul-notile.result matmul-tile.result
LLVM_INSTALL=~/test/polly/llvm_build
alias pollycc="${LLVM_INSTALL}/bin/clang -Xclang -load -Xclang ${LLVM_INSTALL}/lib/LLVMPolly.so"
alias opt="opt -load ${LLVM_INSTALL}/lib/LLVMPolly.so"

${LLVM_INSTALL}/bin/clang -O3 -DTEST matmul-notile.cpp -o matmul-notile
${LLVM_INSTALL}/bin/clang -O3 -DTEST matmul-tile.cpp -o matmul-tile

echo "\nWith print result"
echo "time ./matmul-notile"
time -f "%E real, %U user, %S sys" ./matmul-notile > matmul-notile.result
echo "time ./matmul-tile"
time -f "%E real, %U user, %S sys" ./matmul-tile > matmul-tile.result

${LLVM_INSTALL}/bin/clang -O3 matmul-notile.cpp -o matmul-notile
${LLVM_INSTALL}/bin/clang -O3 matmul-tile.cpp -o matmul-tile

echo "\n\nNo print result"
echo "time ./matmul"
time -f "%E real, %U user, %S sys" ./matmul-notile
echo "time ./matmul-tile"
time -f "%E real, %U user, %S sys" ./matmul-tile

echo "do diff matmul.result matmul.polly.result to ensure the output is same"
diff matmul.result matmul.polly.result


