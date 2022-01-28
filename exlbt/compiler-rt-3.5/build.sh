#!/usr/bin/env bash

currdir=`pwd`

OS=`uname -s`
echo "OS =" ${OS}

TOOLDIR=~/llvm/test/build/bin

INCFLAG="-I$currdir/../../libc/avr-libc-1.8.1/include"

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

bcs="absvdi2.bc absvsi2.bc absvti2.bc adddf3.bc addsf3.bc addtf3.bc addvdi3.bc \
  addvsi3.bc addvti3.bc ashldi3.bc ashlti3.bc ashrdi3.bc \
  ashrti3.bc clear_cache.bc clzdi2.bc clzsi2.bc clzti2.bc cmpdi2.bc cmpti2.bc \
  comparedf2.bc comparesf2.bc ctzdi2.bc ctzsi2.bc ctzti2.bc divdf3.bc \
  divdi3.bc divmoddi4.bc divmodsi4.bc divsf3.bc divsi3.bc divti3.bc divtf3.bc \
  extendsfdf2.bc ffsdi2.bc ffsti2.bc \
  fixdfdi.bc fixdfsi.bc fixdfti.bc fixsfdi.bc fixsfsi.bc fixsfti.bc fixunsdfdi.bc \
  fixunsdfsi.bc fixunsdfti.bc fixunssfdi.bc fixunssfsi.bc fixunssfti.bc \
  fixunsxfdi.bc fixunsxfsi.bc fixunsxfti.bc fixxfdi.bc fixxfti.bc floatundidf.bc \
  floatdisf.bc floatdixf.bc floatsidf.bc floatsisf.bc floattidf.bc floattisf.bc \
  floattixf.bc floatundisf.bc floatundixf.bc floatunsidf.bc \
  floatunsisf.bc floatuntidf.bc floatuntisf.bc floatuntixf.bc \
  int_util.bc lshrdi3.bc lshrti3.bc moddi3.bc \
  modsi3.bc modti3.bc muldc3.bc muldf3.bc muldi3.bc mulodi4.bc mulosi4.bc \
  muloti4.bc mulsc3.bc mulsf3.bc multi3.bc multf3.bc mulvdi3.bc mulvsi3.bc \
  mulvti3.bc mulxc3.bc negdf2.bc negdi2.bc negsf2.bc negti2.bc negvdi2.bc \
  negvsi2.bc negvti2.bc paritydi2.bc paritysi2.bc parityti2.bc \
  popcountdi2.bc popcountsi2.bc popcountti2.bc powidf2.bc \
  powisf2.bc powitf2.bc powixf2.bc subdf3.bc subsf3.bc subvdi3.bc \
  subvsi3.bc subvti3.bc subtf3.bc trampoline_setup.bc truncdfsf2.bc \
  ucmpdi2.bc ucmpti2.bc udivdi3.bc udivmoddi4.bc udivmodsi4.bc udivmodti4.bc \
  udivsi3.bc udivti3.bc umoddi3.bc umodsi3.bc umodti3.bc cpu0-porting.bc"

# All other Chapters
for file in $files
do
  echo "clang++ -target mips-unknown-linux-gnu -c $file.c -emit-llvm $INCFLAG -o $out/$file.bc"
  clang -target mips-unknown-linux-gnu -c $file.c -emit-llvm $INCFLAG -o $out/$file.bc
  ${TOOLDIR}/llc -march=cpu0 -mcpu=cpu032II -relocation-model=static \
  -filetype=obj $out/$file.bc -o $out/$file.o
done

pushd $out
${TOOLDIR}/llvm-link -o libFloat.bc $bcs
${TOOLDIR}/llc -march=cpu0 -mcpu=cpu032II -relocation-model=static \
-filetype=obj libFloat.bc -o libFloat.o
${TOOLDIR}/llvm-dis libFloat.bc -o libFloat.ll
popd

popd

