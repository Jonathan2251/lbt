#!/usr/bin/env bash

source ../functions.sh

sh_name=build-compisite.sh
argNum=$#
arg1=$1
arg2=$2

prologue;

clang -target mips-unknown-linux-gnu -c ../start.cpp -emit-llvm -o start.bc
clang -target mips-unknown-linux-gnu -c ../debug.cpp -emit-llvm -o debug.bc
clang -target mips-unknown-linux-gnu -c ../printf-stdarg-def.c -emit-llvm \
-o printf-stdarg-def.bc
clang -target mips-unknown-linux-gnu -c ../printf-stdarg.c -emit-llvm \
-o printf-stdarg.bc
clang -c ch_Composite.cpp -emit-llvm -o ch_Composite.bc
clang -c ch_Iterator.cpp -emit-llvm -o ch_Iterator.bc
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static -filetype=obj \
start.bc -o start.cpu0.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj debug.bc -o debug.cpu0.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static -filetype=obj \
printf-stdarg-def.bc -o printf-stdarg-def.cpu0.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static -filetype=obj \
printf-stdarg.bc -o printf-stdarg.cpu0.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static -filetype=obj \
ch_Composite.bc -o ch_Composite.cpu0.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static -filetype=obj \
ch_Iterator.bc -o ch_Iterator.cpu0.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static -filetype=obj \
../lib_cpu0.ll -o lib_cpu0.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static -filetype=obj \
lib_designpattern.ll -o lib_designpattern.o
${TOOLDIR}/lld -flavor gnu -target cpu0${endian}-unknown-linux-gnu start.cpu0.o \
debug.cpu0.o printf-stdarg-def.cpu0.o printf-stdarg.cpu0.o ch_Composite.cpu0.o \
ch_Iterator.cpu0.o lib_cpu0.o lib_designpattern.o -o a.out

LBDEXDIR=../../../lbdex
epilogue;
