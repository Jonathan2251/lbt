#!/usr/bin/env bash
OS=`uname -s`
echo "OS =" ${OS}

if [ "$OS" == "Linux" ]; then
  TOOLDIR=/usr/local/llvm/test/cmake_debug_build/bin
else
  TOOLDIR=~/llvm/test/cmake_debug_build/bin/Debug
fi

if [ "$1" != "" ] && [ $1 != cpu032I ] && [ $1 != cpu032II ]; then
  echo "1st argument is cpu032I (default) or cpu032II"
  exit 1
fi
if [ "$1" == "" ]; then
  CPU=cpu032I
else
  CPU=$1
fi
echo "CPU =" "${CPU}"

PWD=`pwd`
pushd ${PWD}
cd ..
bash clean.sh
popd

clang -target mips-unknown-linux-gnu -c ../start.cpp -emit-llvm -o start.bc
clang -target mips-unknown-linux-gnu -c ../printf-stdarg-def.c -emit-llvm \
-o printf-stdarg-def.bc
clang -target mips-unknown-linux-gnu -c ../printf-stdarg.c -emit-llvm \
-o printf-stdarg.bc
clang -c ch_Composite.cpp -emit-llvm -o ch_Composite.bc
clang -c ch_Iterator.cpp -emit-llvm -o ch_Iterator.bc
${TOOLDIR}/llc -march=cpu0 -mcpu=${CPU} -relocation-model=static -filetype=obj \
start.bc -o start.cpu0.o
${TOOLDIR}/llc -march=cpu0 -mcpu=${CPU} -relocation-model=static -filetype=obj \
printf-stdarg-def.bc -o printf-stdarg-def.cpu0.o
${TOOLDIR}/llc -march=cpu0 -mcpu=${CPU} -relocation-model=static -filetype=obj \
printf-stdarg.bc -o printf-stdarg.cpu0.o
${TOOLDIR}/llc -march=cpu0 -mcpu=${CPU} -relocation-model=static -filetype=obj \
ch_Composite.bc -o ch_Composite.cpu0.o
${TOOLDIR}/llc -march=cpu0 -mcpu=${CPU} -relocation-model=static -filetype=obj \
ch_Iterator.bc -o ch_Iterator.cpu0.o
${TOOLDIR}/llc -march=cpu0 -mcpu=${CPU} -relocation-model=static -filetype=obj \
../lib_cpu0.ll -o lib_cpu0.o
${TOOLDIR}/llc -march=cpu0 -mcpu=${CPU} -relocation-model=static -filetype=obj \
lib_designpattern.ll -o lib_designpattern.o
${TOOLDIR}/lld -flavor gnu -target cpu0-unknown-linux-gnu start.cpu0.o \
printf-stdarg-def.cpu0.o printf-stdarg.cpu0.o ch_Composite.cpu0.o \
ch_Iterator.cpu0.o lib_cpu0.o lib_designpattern.o -o a.out
${TOOLDIR}/llvm-objdump -elf2hex a.out > ../../cpu0_verilog/cpu0.hex
