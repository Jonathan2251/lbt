#!/usr/bin/env bash

source functions.sh

sh_name=build-slinker.sh
argNum=$#
arg1=$1
arg2=$2

prologue;

clang -target mips-unknown-linux-gnu -c start.cpp -emit-llvm -o start.bc
clang -target mips-unknown-linux-gnu -c debug.cpp -emit-llvm -o debug.bc
clang -target mips-unknown-linux-gnu -c printf-stdarg-def.c -emit-llvm \
-o printf-stdarg-def.bc
clang -target mips-unknown-linux-gnu -c printf-stdarg.c -emit-llvm \
-o printf-stdarg.bc
clang -target mips-unknown-linux-gnu -c ${LBDEXDIR}/input/ch4_1_2.cpp \
-emit-llvm -o ch4_1_2.bc
clang -target mips-unknown-linux-gnu -c ${LBDEXDIR}/input/ch8_1_5.cpp \
-emit-llvm -o ch8_1_5.bc
clang -O1 -target mips-unknown-linux-gnu -c ${LBDEXDIR}/input/ch8_1_phinode.cpp \
-emit-llvm -o ch8_1_phinode.bc
clang -target mips-unknown-linux-gnu -c ${LBDEXDIR}/input/ch8_blockaddr.cpp \
-emit-llvm -o ch8_blockaddr.bc
clang -target mips-unknown-linux-gnu -c ${LBDEXDIR}/input/ch8_2_longbranch.cpp \
-emit-llvm -o ch8_2_longbranch.bc
clang -target mips-unknown-linux-gnu -c ${LBDEXDIR}/input/ch9_3_detect_exception.cpp \
-emit-llvm -o ch9_3_detect_exception.bc
clang -c ${LBDEXDIR}/input/ch9_4.cpp -emit-llvm -o ch9_4.bc
clang -I${LBDEXDIR}/input/ -target mips-unknown-linux-gnu -c ch_slinker.cpp \
-emit-llvm -o ch_slinker.bc
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj start.bc -o start.cpu0.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj debug.bc -o debug.cpu0.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj printf-stdarg-def.bc -o printf-stdarg-def.cpu0.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj printf-stdarg.bc -o printf-stdarg.cpu0.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj -cpu0-enable-overflow=true ch4_1_2.bc -o ch4_1_2.cpu0.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj ch8_1_5.bc -o ch8_1_5.cpu0.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj ch8_1_phinode.bc -o ch8_1_phinode.cpu0.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj ch8_blockaddr.bc -o ch8_blockaddr.cpu0.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=pic \
-filetype=obj -force-cpu0-long-branch ch8_2_longbranch.bc -o \
ch8_2_longbranch.cpu0.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj ch9_3_detect_exception.bc -o ch9_3_detect_exception.cpu0.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj ch9_4.bc -o ch9_4.cpu0.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj ch_slinker.bc -o ch_slinker.cpu0.o
${TOOLDIR}/llc -march=cpu0${endian} -mcpu=${CPU} -relocation-model=static \
-filetype=obj lib_cpu0.ll -o lib_cpu0.o
${TOOLDIR}/lld -flavor gnu -target cpu0${endian}-unknown-linux-gnu start.cpu0.o \
debug.cpu0.o printf-stdarg-def.cpu0.o printf-stdarg.cpu0.o ch4_1_2.cpu0.o \
ch8_1_5.cpu0.o ch8_1_phinode.cpu0.o ch8_blockaddr.cpu0.o ch8_2_longbranch.cpu0.o \
ch9_3_detect_exception.cpu0.o ch9_4.cpu0.o ch_slinker.cpu0.o lib_cpu0.o -o a.out

epilogue;

