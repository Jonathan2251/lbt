#!/usr/bin/env bash

OS=`uname -s`
echo "OS =" ${OS}

if [ "$OS" == "Linux" ]; then
  TOOLDIR=~/llvm/test/cmake_debug_build/bin
  INCFLAG="-I/home/cschen/test/open-src-libc/avr-libc-1.8.1/include -I/home/cschen/test/open-src-libc/avr-libc-1.8.1/common"
else
  TOOLDIR=~/llvm/test/cmake_debug_build/Debug/bin
  INCFLAG="-I/Users/Jonathan/test/avr-libc-1.8.1/include -I/Users/Jonathan/test/avr-libc-1.8.1/common"
fi

out=obj

rm -rf $out
mkdir $out

files="abort assert atoi bsearch ctype getenv malloc rand realloc strtol system \
abs atexit atol calloc errno labs qsort random strtod strtoul"

bcs="abort.bc assert.bc atoi.bc bsearch.bc ctype.bc getenv.bc malloc.bc rand.bc realloc.bc strtol.bc system.bc \
abs.bc atexit.bc atol.bc calloc.bc errno.bc labs.bc qsort.bc random.bc strtod.bc strtoul.bc"

# All other Chapters
for file in $files
do
  echo $file.c
  clang -target mips-unknown-linux-gnu -c $file.c -emit-llvm $INCFLAG -o $out/$file.bc
done

pushd $out
${TOOLDIR}/llvm-link -o stdlib.bc $bcs
${TOOLDIR}/llc -march=cpu0 -mcpu=cpu032II -relocation-model=static \
-filetype=obj stdlib.bc -o stdlib.o
popd

