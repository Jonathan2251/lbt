#!/usr/bin/env bash

INCFLAG="-I../libsoftfloat/compiler-rt/builtins"

source functions.sh

sh_name=build-float.sh
argNum=$#
arg1=$1
arg2=$2

prologue;

libsf=../libsoftfloat/compiler-rt
pushd ${libsf}
bash build.sh
popd
olibsf=${libsf}/obj

${CLANG} -target mips-unknown-linux-gnu -c start.cpp -emit-llvm -o start.bc
${CLANG} -target mips-unknown-linux-gnu -c debug.cpp -emit-llvm -o debug.bc
${CLANG} -target mips-unknown-linux-gnu -c printf-stdarg-def.c -emit-llvm \
-o printf-stdarg-def.bc
${CLANG} -target mips-unknown-linux-gnu -c printf-stdarg.c -emit-llvm \
-o printf-stdarg.bc
${CLANG} $INCFLAG -c ch_float_necessary.cpp -emit-llvm -o ch_float_necessary.bc
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj -has-lld=true start.bc -o start.cpu0.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj -has-lld=true debug.bc -o debug.cpu0.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj -has-lld=true printf-stdarg-def.bc -o printf-stdarg-def.cpu0.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj -has-lld=true printf-stdarg.bc -o printf-stdarg.cpu0.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj -has-lld=true ch_float_necessary.bc -o ch_float_necessary.cpu0.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj -has-lld=true lib_cpu0.ll -o lib_cpu0.o
${TOOLDIR}/lld -flavor gnu -o a.out \
  start.cpu0.o debug.cpu0.o printf-stdarg-def.cpu0.o printf-stdarg.cpu0.o \
  ch_float_necessary.cpu0.o lib_cpu0.o ${olibsf}/libFloat.o
#  ${olibsf}/fixsfsi.o ${olibsf}/fixsfdi.o ${olibsf}/fixdfsi.o \
#  ${olibsf}/addsf3.o ${olibsf}/mulsf3.o ${olibsf}/divsf3.o \
#  ${olibsf}/adddf3.o ${olibsf}/muldf3.o ${olibsf}/divdf3.o \
#  ${olibsf}/ashrdi3.o ${olibsf}/ashldi3.o ${olibsf}/lshrdi3.o \
#  ${olibsf}/extendsfdf2.o ${olibsf}/truncdfsf2.o


epilogue;

