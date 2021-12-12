#!/usr/bin/env bash

source functions.sh

sh_name=build-slinker-2.sh
argNum=$#
arg1=$1
arg2=$2

prologue;

# -mcpu cannot pass to llvm via -mllvm while -has-lld can. So add cpu032I and cpu032II in 
# clang/include/clang/Driver/Options.td

${CLANG} -target cpu0${endian}-unknown-linux-gnu -c start.cpp -static \
  -fintegrated-as -o start.o  -mcpu=${CPU} -mllvm -has-lld=true
${CLANG} -target cpu0${endian}-unknown-linux-gnu -c debug.cpp -static \
  -fintegrated-as -o debug.o -mcpu=${CPU} -mllvm -has-lld=true
${CLANG} -target cpu0${endian}-unknown-linux-gnu -c printf-stdarg-def.c -static \
  -fintegrated-as -o printf-stdarg-def.o -mcpu=${CPU} -mllvm -has-lld=true
${CLANG} -target cpu0${endian}-unknown-linux-gnu -c printf-stdarg.c -static \
  -fintegrated-as -o printf-stdarg.o -mcpu=${CPU} -mllvm -has-lld=true
${CLANG} -target cpu0${endian}-unknown-linux-gnu -c \
  ${LBDEXDIR}/input/ch4_1_addsuboverflow.cpp -static -fintegrated-as \
  -o ch4_1_addsuboverflow.o -mcpu=${CPU} -mllvm -has-lld=true
${CLANG} -target cpu0${endian}-unknown-linux-gnu -c \
  ${LBDEXDIR}/input/ch8_1_br_jt.cpp -static -fintegrated-as -o ch8_1_br_jt.o \
  -mcpu=${CPU} -mllvm -has-lld=true
${CLANG} -O3 -target cpu0${endian}-unknown-linux-gnu -c \
  ${LBDEXDIR}/input/ch8_2_phinode.cpp -static -fintegrated-as -o ch8_2_phinode.o \
  -mcpu=${CPU} -mllvm -has-lld=true
${CLANG} -target cpu0${endian}-unknown-linux-gnu -c \
  ${LBDEXDIR}/input/ch8_1_blockaddr.cpp -static -fintegrated-as \
  -o ch8_1_blockaddr.o -mcpu=${CPU} -mllvm -has-lld=true
${CLANG} -target cpu0${endian}-unknown-linux-gnu -c \
  ${LBDEXDIR}/input/ch8_2_longbranch.cpp -static -fintegrated-as \
  -o ch8_2_longbranch.o -mcpu=${CPU} -mllvm -has-lld=true
${CLANG} -O1 -target cpu0${endian}-unknown-linux-gnu -c \
  ${LBDEXDIR}/input/ch9_2_tailcall.cpp -static -fintegrated-as \
  -o ch9_2_tailcall.o -mcpu=${CPU} -mllvm -has-lld=true
${CLANG} -target cpu0${endian}-unknown-linux-gnu -c \
${LBDEXDIR}/input/ch9_3_detect_exception.cpp -static -fintegrated-as \
  -o ch9_3_detect_exception.o -mcpu=${CPU} -mllvm -has-lld=true
${CLANG} -I${LBDEXDIR}/input/ -target cpu0${endian}-unknown-linux-gnu \
  -c ch_slinker.cpp -static -fintegrated-as -o ch_slinker.o -mcpu=${CPU} \
  -mllvm -has-lld=true

${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
  -filetype=obj -has-lld=true lib_cpu0.ll -o lib_cpu0.o

${TOOLDIR}/lld -flavor gnu start.o \
  debug.o printf-stdarg-def.o printf-stdarg.o ch4_1_addsuboverflow.o \
  ch8_1_br_jt.o ch8_2_phinode.o ch8_1_blockaddr.o ch8_2_longbranch.o \
  ch9_2_tailcall.o ch9_3_detect_exception.o ch_slinker.o lib_cpu0.o -o a.out

epilogue;

