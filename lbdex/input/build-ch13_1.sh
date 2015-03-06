#!/usr/bin/env bash

source functions.sh

sh_name=build-ch13_1.sh
argNum=$#
arg1=$1
arg2=$2

prologue;

clang -target mips-unknown-linux-gnu -c start.cpp -emit-llvm -o start.bc
clang -target mips-unknown-linux-gnu -c ch13_1.cpp -emit-llvm -o ch13_1.bc
clang -target mips-unknown-linux-gnu -c a.cpp -emit-llvm -o a.bc
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj start.bc -o start.cpu0.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj lib_cpu0.ll -o lib_cpu0.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj ch13_1.bc -o ch13_1.cpu0.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj a.bc -o a.cpu0.o
${TOOLDIR}/lld -flavor gnu -target cpu0${endian}-unknown-linux-gnu \
start.cpu0.o lib_cpu0.o ch13_1.cpu0.o a.cpu0.o -o a.out

epilogue;

