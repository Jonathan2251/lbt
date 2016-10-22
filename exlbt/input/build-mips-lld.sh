#!/usr/bin/env bash

endian=
#endian=el

OS=`uname -s`
echo "OS =" ${OS}

if [ "$OS" == "Linux" ]; then
  TOOLDIR=~/llvm/release/cmake_debug_build/bin
  CLANG=~/llvm/release/cmake_debug_build/bin/clang
  LLD=~/llvm/test/cmake_debug_build/bin/lld
else
  TOOLDIR=~/llvm/release/cmake_debug_build/Debug/bin
  CLANG=~/llvm/release/cmake_debug_build/Debug/bin/clang
  LLD=~/llvm/test/cmake_debug_build/Debug/bin/lld
fi

bash clean.sh

${CLANG} -target mips-unknown-linux-gnu -c printf-stdarg-def.c -emit-llvm \
-o printf-stdarg-def.bc
${CLANG} -target mips-unknown-linux-gnu -c printf-stdarg.c -emit-llvm \
-o printf-stdarg.bc
${CLANG} -target mips-unknown-linux-gnu -c ch_hello.c -emit-llvm -DTEST_RUN -o ch_hello.bc
${TOOLDIR}/llc -march=mips${endian} -relocation-model=static -filetype=obj \
printf-stdarg-def.bc -o printf-stdarg-def.o
${TOOLDIR}/llc -march=mips${endian} -relocation-model=static -filetype=obj \
printf-stdarg.bc -o printf-stdarg.o
${TOOLDIR}/llc -march=mips${endian} -relocation-model=static -filetype=obj \
ch_hello.bc -o ch_hello.o
${TOOLDIR}/llc -march=mips${endian} -relocation-model=static -filetype=obj \
lib_cpu0.ll -o lib_cpu0.o
${LLD} -flavor gnu \
printf-stdarg-def.o printf-stdarg.o ch_hello.o \
lib_cpu0.o
