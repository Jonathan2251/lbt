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

files="memcpy memset strcpy strlen strdup strlcat strlcpy strtok strtok_r"

bcs="memcpy.bc memset.bc strcpy.bc strlen.bc strdup.bc strlcat.bc strlcpy.bc strtok.bc strtok_r.bc"

# All other Chapters
for file in $files
do
  echo $file.c
  clang -target mips-unknown-linux-gnu -c $file.c -emit-llvm $INCFLAG -o $out/$file.bc
done

pushd $out
${TOOLDIR}/llvm-link -o string.bc $bcs
${TOOLDIR}/llc -march=cpu0 -mcpu=cpu032II -relocation-model=static \
-filetype=obj string.bc -o string.o
popd

