#!/usr/bin/env bash

INCFLAG="-I../libsoftfloat/compiler-rt/builtins"

source functions.sh

sh_name=build-float-necessary-2.sh
argNum=$#
arg1=$1
arg2=$2

prologue;

${CLANG} -target cpu0${endian}-unknown-linux-gnu -c start.cpp -static \
  -fintegrated-as -o start.o -mcpu=${CPU} -mllvm -has-lld=true
${CLANG} -target cpu0${endian}-unknown-linux-gnu -c debug.cpp -static \
  -fintegrated-as -o debug.o -mcpu=${CPU} -mllvm -has-lld=true
${CLANG} -target cpu0${endian}-unknown-linux-gnu -c printf-stdarg-def.c -static \
  -fintegrated-as -o printf-stdarg-def.o -mcpu=${CPU} -mllvm -has-lld=true
${CLANG} -target cpu0${endian}-unknown-linux-gnu -c printf-stdarg.c -static \
  -fintegrated-as -o printf-stdarg.o -mcpu=${CPU} -mllvm -has-lld=true
${CLANG} -target cpu0${endian}-unknown-linux-gnu $INCFLAG \
  -c ch_float_necessary.cpp -static -fintegrated-as -o ch_float_necessary.o \
  -mcpu=${CPU} -mllvm -has-lld=true

${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
  -filetype=obj -has-lld=true lib_cpu0.ll -o lib_cpu0.o

libsf=../libsoftfloat/compiler-rt
pushd ${libsf}
#bash build-2.sh $1
make CPU=$1 endian=${endian}
popd
olibsf=${libsf}/build

${TOOLDIR}/lld -flavor gnu -o a.out start.o debug.o printf-stdarg-def.o \
  printf-stdarg.o ch_float_necessary.o lib_cpu0.o ${olibsf}/libFloat.a
#  ${olibsf}/fixsfsi.o ${olibsf}/fixsfdi.o ${olibsf}/fixdfsi.o \
#  ${olibsf}/addsf3.o ${olibsf}/mulsf3.o ${olibsf}/divsf3.o \
#  ${olibsf}/adddf3.o ${olibsf}/muldf3.o ${olibsf}/divdf3.o \
#  ${olibsf}/ashrdi3.o ${olibsf}/ashldi3.o ${olibsf}/lshrdi3.o \
#  ${olibsf}/extendsfdf2.o ${olibsf}/truncdfsf2.o

epilogue;

