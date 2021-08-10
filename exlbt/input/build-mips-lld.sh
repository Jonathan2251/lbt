#!/usr/bin/env bash

endian=
#endian=el

OS=`uname -s`
echo "OS =" ${OS}

TOOLDIR=~/llvm/test/build/bin
CLANG=~/llvm/test/build/bin/clang
LLD=~/llvm/test/build/bin/lld

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
