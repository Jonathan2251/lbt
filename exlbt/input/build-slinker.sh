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
${CLANG} -target mips-unknown-linux-gnu -c \
${LBDEXDIR}/input/ch4_1_addsuboverflow.cpp -emit-llvm -o ch4_1_addsuboverflow.bc
${CLANG} -target mips-unknown-linux-gnu -c \
${LBDEXDIR}/input/ch8_1_br_jt.cpp -emit-llvm -o ch8_1_br_jt.bc
${CLANG} -O3 -target mips-unknown-linux-gnu -c \
${LBDEXDIR}/input/ch8_2_phinode.cpp -emit-llvm -o ch8_2_phinode.bc
${CLANG} -target mips-unknown-linux-gnu -c \
${LBDEXDIR}/input/ch8_1_blockaddr.cpp -emit-llvm -o ch8_1_blockaddr.bc
${CLANG} -target mips-unknown-linux-gnu -c \
${LBDEXDIR}/input/ch8_2_longbranch.cpp -emit-llvm -o ch8_2_longbranch.bc
${CLANG} -O1 -target mips-unknown-linux-gnu -c \
${LBDEXDIR}/input/ch9_2_tailcall.cpp -emit-llvm -o ch9_2_tailcall.bc
${CLANG} -target mips-unknown-linux-gnu -c \
${LBDEXDIR}/input/ch9_3_detect_exception.cpp -emit-llvm -o \
ch9_3_detect_exception.bc
${CLANG} -I${LBDEXDIR}/input/ -target mips-unknown-linux-gnu -c \
ch_slinker.cpp -emit-llvm -o ch_slinker.bc
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj start.bc -o start.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj debug.bc -o debug.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj printf-stdarg-def.bc -o printf-stdarg-def.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj printf-stdarg.bc -o printf-stdarg.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj -cpu0-enable-overflow=true ch4_1_addsuboverflow.bc -o \
ch4_1_addsuboverflow.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj ch8_1_br_jt.bc -o ch8_1_br_jt.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj ch8_2_phinode.bc -o ch8_2_phinode.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj ch8_1_blockaddr.bc -o ch8_1_blockaddr.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=pic \
-filetype=obj -force-cpu0-long-branch ch8_2_longbranch.bc -o \
ch8_2_longbranch.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj -enable-cpu0-tail-calls ch9_2_tailcall.bc -o ch9_2_tailcall.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj ch9_3_detect_exception.bc -o ch9_3_detect_exception.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj ch_slinker.bc -o ch_slinker.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj lib_cpu0.ll -o lib_cpu0.o
${TOOLDIR}/lld -flavor gnu start.o \
debug.o printf-stdarg-def.o printf-stdarg.o ch4_1_addsuboverflow.o \
ch8_1_br_jt.o ch8_2_phinode.o ch8_1_blockaddr.o ch8_2_longbranch.o \
ch9_2_tailcall.o ch9_3_detect_exception.o ch_slinker.o lib_cpu0.o -o a.out

epilogue;

