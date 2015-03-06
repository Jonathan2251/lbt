#!/usr/bin/env bash
OS=`uname -s`
echo "OS =" ${OS}

if [ "$OS" == "Linux" ]; then
  TOOLDIR=/usr/local/llvm/test/cmake_debug_build/bin
else
  TOOLDIR=~/test/lld/cmake_debug_build/Debug/bin
fi

bash clean.sh

clang -target mips-unknown-linux-gnu -c printf-stdarg-def.c -emit-llvm \
-o printf-stdarg-def.bc
clang -target mips-unknown-linux-gnu -c printf-stdarg.c -emit-llvm \
-o printf-stdarg.bc
clang -c ch_c++_1.cpp -emit-llvm -DTEST_RUN -o ch_c++_1.bc
clang -O1 -target mips-unknown-linux-gnu -c ch12_4.cpp -emit-llvm -o ch12_4.bc
${TOOLDIR}/llc -march=mips -relocation-model=static -filetype=obj \
printf-stdarg-def.bc -o printf-stdarg-def.o
${TOOLDIR}/llc -march=mips -relocation-model=static -filetype=obj \
printf-stdarg.bc -o printf-stdarg.o
${TOOLDIR}/llc -march=mips -relocation-model=static -filetype=obj \
ch_c++_1.bc -o ch_c++_1.o
${TOOLDIR}/llc -march=mips -relocation-model=static -filetype=obj \
lib_cpu0.ll -o lib_cpu0.o
${TOOLDIR}/lld -flavor gnu -target mipsel-unknown-linux-gnu \
printf-stdarg-def.o printf-stdarg.o ch_c++_1.o lib_cpu0.o \
