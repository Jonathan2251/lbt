#!/usr/bin/env bash

OS=`uname -s`
echo "OS =" ${OS}

if [ "$OS" == "Linux" ]; then
  TOOLDIR=/usr/local/llvm/test/cmake_debug_build/bin
else
  TOOLDIR=~/llvm/test/cmake_debug_build/Debug/bin
fi

INCFLAG="-I../../include -I../../common"
out=obj

rm -rf $out
mkdir $out

files="fgetc fputs getchar puts_p sprintf ungetc vfscanf_p vsprintf_p clearerr \
fgets fputs_p printf sprintf_p vfprintf vprintf fclose fread gets printf_p \
scanf sscanf vfprintf vscanf fdevopen fprintf fscanf iob putchar scanf_p \
sscanf_p vfprintf_p vsnprintf feof fprintf_p fscanf_p snprintf vsnprintf_p \
ferror fputc fwrite puts snprintf_p vfscanf vsprintf"

bcs="fgetc.bc fputs.bc getchar.bc puts_p.bc sprintf.bc ungetc.bc vfscanf_p.bc vsprintf_p.bc clearerr.bc \
fgets.bc fputs_p.bc printf.bc sprintf_p.bc vfprintf.bc vprintf.bc fclose.bc fread.bc gets.bc printf_p.bc \
scanf.bc sscanf.bc vfprintf.bc vscanf.bc fdevopen.bc fprintf.bc fscanf.bc iob.bc putchar.bc scanf_p.bc \
sscanf_p.bc vfprintf_p.bc vsnprintf.bc feof.bc fprintf_p.bc fscanf_p.bc snprintf.bc vsnprintf_p.bc \
ferror.bc fputc.bc fwrite.bc puts.bc snprintf_p.bc vfscanf.bc vsprintf.bc"

objs="fgetc.o fputs.o getchar.o puts_p.o sprintf.o ungetc.o vfscanf_p.o vsprintf_p.o clearerr.o \
fgets.o fputs_p.o printf.o sprintf_p.o vfprintf.o vprintf.o fclose.o fread.o gets.o printf_p.o \
scanf.o sscanf.o vfprintf.o vscanf.o fdevopen.o fprintf.o fscanf.o iob.o putchar.o scanf_p.o \
sscanf_p.o vfprintf_p.o vsnprintf.o feof.o fprintf_p.o fscanf_p.o snprintf.o vsnprintf_p.o \
ferror.o fputc.o fwrite.o puts.o snprintf_p.o vfscanf.o vsprintf.o"

clang -target mips-unknown-linux-gnu -c start.cpp -emit-llvm -o $out/start.bc
clang -target mips-unknown-linux-gnu -c debug.cpp -emit-llvm -o $out/debug.bc

${TOOLDIR}/llc -march=cpu0 -mcpu=cpu032II -relocation-model=static \
-filetype=obj $out/start.bc -o $out/start.o
${TOOLDIR}/llc -march=cpu0 -mcpu=cpu032II -relocation-model=static \
-filetype=obj $out/debug.bc -o $out/debug.o

# All other Chapters
for file in $files
do
  clang -target mips-unknown-linux-gnu -c $file.c -emit-llvm $INCFLAG -o $out/$file.bc
done

pushd $out
${TOOLDIR}/llvm-link -o stdio.bc $bcs
${TOOLDIR}/llc -march=cpu0 -mcpu=cpu032II -relocation-model=static \
-filetype=obj stdio.bc -o stdio.o
${TOOLDIR}/lld -flavor gnu -target cpu0-unknown-linux-gnu start.o \
debug.o stdio.o ../../stdlib/obj/stdlib.o ../../string/obj/string.o -o stdio.out
popd
