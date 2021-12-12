#!/usr/bin/env bash

currdir=`pwd`

OS=`uname -s`
echo "OS =" ${OS}

CLANG=~/llvm/test/build/bin/clang
TOOLDIR=~/llvm/test/build/bin

INCFLAG="-I$currdir/../../libc/avr-libc-1.8.1/include"

CPU=$1

pushd builtins

out=${currdir}/obj

rm -rf $out
mkdir $out

files="absvdi2 absvsi2 absvti2 adddf3 addsf3 addtf3 addvdi3 \
  addvsi3 addvti3 ashldi3 ashlti3 ashrdi3 \
  ashrti3 clear_cache clzdi2 clzsi2 clzti2 cmpdi2 cmpti2 \
  comparedf2 comparesf2 ctzdi2 ctzsi2 ctzti2 divdf3 \
  divdi3 divmoddi4 divmodsi4 divsf3 divsi3 divti3 divtf3 \
  extendsfdf2 ffsdi2 ffsti2 \
  fixdfdi fixdfsi fixdfti fixsfdi fixsfsi fixsfti fixunsdfdi \
  fixunsdfsi fixunsdfti fixunssfdi fixunssfsi fixunssfti \
  fixunsxfdi fixunsxfsi fixunsxfti fixxfdi fixxfti \
  floatdisf floatdixf floatsidf floatsisf floattidf floattisf \
  floattixf floatundidf floatundisf floatundixf floatunsidf \
  floatunsisf floatuntidf floatuntisf floatuntixf \
  int_util lshrdi3 lshrti3 moddi3 \
  modsi3 modti3 muldc3 muldf3 muldi3 mulodi4 mulosi4 \
  muloti4 mulsc3 mulsf3 multi3 multf3 mulvdi3 mulvsi3 \
  mulvti3 mulxc3 negdf2 negdi2 negsf2 negti2 negvdi2 \
  negvsi2 negvti2 paritydi2 paritysi2 parityti2 \
  popcountdi2 popcountsi2 popcountti2 powidf2 \
  powisf2 powitf2 powixf2 subdf3 subsf3 subvdi3 \
  subvsi3 subvti3 subtf3 trampoline_setup truncdfsf2 \
  ucmpdi2 ucmpti2 udivdi3 udivmoddi4 udivmodsi4 udivmodti4 \
  udivsi3 udivti3 umoddi3 umodsi3 umodti3 cpu0-porting"

objs="absvdi2.o absvsi2.o absvti2.o adddf3.o addsf3.o addtf3.o addvdi3.o \
  addvsi3.o addvti3.o ashldi3.o ashlti3.o ashrdi3.o \
  ashrti3.o clear_cache.o clzdi2.o clzsi2.o clzti2.o cmpdi2.o cmpti2.o \
  comparedf2.o comparesf2.o ctzdi2.o ctzsi2.o ctzti2.o divdf3.o \
  divdi3.o divmoddi4.o divmodsi4.o divsf3.o divsi3.o divti3.o divtf3.o \
  extendsfdf2.o ffsdi2.o ffsti2.o \
  fixdfdi.o fixdfsi.o fixdfti.o fixsfdi.o fixsfsi.o fixsfti.o fixunsdfdi.o \
  fixunsdfsi.o fixunsdfti.o fixunssfdi.o fixunssfsi.o fixunssfti.o \
  fixunsxfdi.o fixunsxfsi.o fixunsxfti.o fixxfdi.o fixxfti.o floatundidf.o \
  floatdisf.o floatdixf.o floatsidf.o floatsisf.o floattidf.o floattisf.o \
  floattixf.o floatundisf.o floatundixf.o floatunsidf.o \
  floatunsisf.o floatuntidf.o floatuntisf.o floatuntixf.o \
  int_util.o lshrdi3.o lshrti3.o moddi3.o \
  modsi3.o modti3.o muldc3.o muldf3.o muldi3.o mulodi4.o mulosi4.o \
  muloti4.o mulsc3.o mulsf3.o multi3.o multf3.o mulvdi3.o mulvsi3.o \
  mulvti3.o mulxc3.o negdf2.o negdi2.o negsf2.o negti2.o negvdi2.o \
  negvsi2.o negvti2.o paritydi2.o paritysi2.o parityti2.o \
  popcountdi2.o popcountsi2.o popcountti2.o powidf2.o \
  powisf2.o powitf2.o powixf2.o subdf3.o subsf3.o subvdi3.o \
  subvsi3.o subvti3.o subtf3.o trampoline_setup.o truncdfsf2.o \
  ucmpdi2.o ucmpti2.o udivdi3.o udivmoddi4.o udivmodsi4.o udivmodti4.o \
  udivsi3.o udivti3.o umoddi3.o umodsi3.o umodti3.o cpu0-porting.o"

# All other Chapters
for file in $files
do
  echo "clang++ -target cpu0${endian}-unknown-linux-gnu -c $file.c -static \
    -fintegrated-as $INCFLAG -o $out/$file.o"
  ${CLANG} -target cpu0${endian}-unknown-linux-gnu -c $file.c -static \
    -fintegrated-as $INCFLAG -o $out/$file.o -mcpu=${CPU} -mllvm -has-lld=true
done

pushd $out
ar -rcs libFloat.a $objs ../../../input/printf-stdarg-def.o ../../../input/printf-stdarg.o
popd

popd

