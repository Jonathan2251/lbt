#!/usr/bin/env bash

source functions.sh

sh_name=build-slinker.sh
argNum=$#
arg1=$1
arg2=$2

prologue;

${CLANG} -target mips-unknown-linux-gnu -c start.cpp -emit-llvm -o \
start.bc
${CLANG} -target mips-unknown-linux-gnu -c debug.cpp -emit-llvm -o \
debug.bc
${CLANG} -target mips-unknown-linux-gnu -c printf-stdarg-def.c \
-emit-llvm -o printf-stdarg-def.bc
${CLANG} -target mips-unknown-linux-gnu -c printf-stdarg.c -emit-llvm \
-o printf-stdarg.bc
${CLANG} -I${LBDEXDIR}/input/ -target mips-unknown-linux-gnu -c \
ch_slinker.cpp -emit-llvm -o ch_slinker.bc
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj -has-lld=true start.bc -o start.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj -has-lld=true debug.bc -o debug.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj -has-lld=true printf-stdarg-def.bc -o printf-stdarg-def.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj printf-stdarg.bc -o printf-stdarg.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj -has-lld=true ch_slinker.bc -o ch_slinker.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj -has-lld=true lib_cpu0.ll -o lib_cpu0.o
${TOOLDIR}/lld -flavor gnu start.o \
debug.o printf-stdarg-def.o printf-stdarg.o ch_slinker.o lib_cpu0.o -o a.out

epilogue;

