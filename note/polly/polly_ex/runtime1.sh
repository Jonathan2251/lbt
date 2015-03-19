#!/bin/sh -a

rm matmul matmul1
LLVM_INSTALL=~/test/polly/llvm_build

${LLVM_INSTALL}/bin/clang -O3 -DTEST matmul.cpp -o matmul
${LLVM_INSTALL}/bin/clang -O3 -DTEST matmul1.cpp -o matmul1

echo "\nWith print result"
echo "time ./matmul"
time -f "%E real, %U user, %S sys" ./matmul > matmul.result
echo "time ./matmul1"
time -f "%E real, %U user, %S sys" ./matmul1 > matmul1.result

${LLVM_INSTALL}/bin/clang -O3 matmul.cpp -o matmul
${LLVM_INSTALL}/bin/clang -O3 matmul1.cpp -o matmul1
g++ -O3 matmul.cpp -o matmul-gcc

echo "\n\nNo print result"
echo "time ./matmul"
time -f "%E real, %U user, %S sys" ./matmul
echo "time ./matmul1"
time -f "%E real, %U user, %S sys" ./matmul1
echo "time ./matmul-gcc"
time -f "%E real, %U user, %S sys" ./matmul-gcc

echo "do diff matmul.result matmul1.result to ensure the output is same"
diff matmul.result matmul1.result


