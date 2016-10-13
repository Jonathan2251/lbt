#!/usr/bin/env bash

#endian=
endian=el

OS=`uname -s`
echo "OS =" ${OS}

if [ "$OS" == "Linux" ]; then
  TOOLDIR=~/llvm/release/cmake_release_build/bin
else
  TOOLDIR=~/llvm/release/cmake_release_build/Debug/bin
fi

bash clean.sh

clang -target mips-unknown-linux-gnu -c printf-stdarg-def.c -emit-llvm \
-o printf-stdarg-def.bc
clang -target mips-unknown-linux-gnu -c printf-stdarg.c -emit-llvm \
-o printf-stdarg.bc
clang -target mips-unknown-linux-gnu -c ch_hello.c -emit-llvm -DTEST_RUN -o ch_hello.bc
${TOOLDIR}/llc -march=mips${endian} -relocation-model=static -filetype=obj \
printf-stdarg-def.bc -o printf-stdarg-def.o
${TOOLDIR}/llc -march=mips${endian} -relocation-model=static -filetype=obj \
printf-stdarg.bc -o printf-stdarg.o
${TOOLDIR}/llc -march=mips${endian} -relocation-model=static -filetype=obj \
ch_hello.bc -o ch_hello.o
${TOOLDIR}/llc -march=mips${endian} -relocation-model=static -filetype=obj \
lib_cpu0.ll -o lib_cpu0.o
~/llvm/test/cmake_debug_build/bin/lld -flavor gnu \
printf-stdarg-def.o printf-stdarg.o ch_hello.o \
lib_cpu0.o
