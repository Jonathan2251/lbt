#!/usr/bin/env bash

# for example:
# bash make.sh cpu032II be Makefile.builtins
# bash make.sh cpu032I le Makefile.slinker
# bash make.sh cpu032II be Makefile.float-necessary
# bash make.sh cpu032II be Makefile.printf-stdarg-2
# bash make.sh cpu032II be Makefile.ch13_1

ARG_NUM=$#
ENDIAN=

prologue() {
  LBDEXDIR=../../lbdex

  if [ $ARG_NUM == 0 ]; then
    echo "useage: bash $sh_name cpu_type ENDIAN"
    echo "  cpu_type: cpu032I or cpu032II"
    echo "  ENDIAN: be (big ENDIAN, default) or le (little ENDIAN)"
    echo "for example:"
    echo "  bash build-slinker.sh cpu032I be"
    exit 1;
  fi
  if [ $1 != cpu032I ] && [ $1 != cpu032II ]; then
    echo "1st argument is cpu032I or cpu032II"
    exit 1
  fi

  INCDIR=../../lbdex/input
  OS=`uname -s`
  echo "OS =" ${OS}

  TOOLDIR=~/llvm/test/build/bin
  CLANG=~/llvm/test/build/bin/clang

  CPU=$1
  echo "CPU =" "${CPU}"

  if [ "$2" != "" ] && [ $2 != le ] && [ $2 != be ]; then
    echo "2nd argument is be (big ENDIAN, default) or le (little ENDIAN)"
    exit 1
  fi
  if [ "$2" == "" ] || [ $2 == be ]; then
    ENDIAN=
  else
    ENDIAN=el
  fi
  echo "ENDIAN =" "${ENDIAN}"

  bash clean.sh
  builtins="../libsoftfloat/compiler-rt/builtins"
  if [ ! -L $builtins ]; then
    ln -s $HOME/llvm/llvm-project/compiler-rt/lib/builtins $builtins
  fi
}

isLittleEndian() {
  echo "ENDIAN = " "$ENDIAN"
  if [ "$ENDIAN" == "LittleEndian" ] ; then
    le="true"
  elif [ "$ENDIAN" == "BigEndian" ] ; then
    le="false"
  else
    echo "!ENDIAN unknown"
    exit 1
  fi
}

elf2hex() {
  ${TOOLDIR}/elf2hex -le=${le} a.out > ${LBDEXDIR}/verilog/cpu0.hex
  if [ ${le} == "true" ] ; then
    echo "1   /* 0: big ENDIAN, 1: little ENDIAN */" > ${LBDEXDIR}/verilog/cpu0.config
  else
    echo "0   /* 0: big ENDIAN, 1: little ENDIAN */" > ${LBDEXDIR}/verilog/cpu0.config
  fi
  cat ${LBDEXDIR}/verilog/cpu0.config
}

epilogue() {
  ENDIAN=`${TOOLDIR}/llvm-readobj -h a.out|grep "DataEncoding"|awk '{print $2}'`
  isLittleEndian;
  elf2hex;
}

FILE=$3

if [ ! -f "$FILE" ]; then
  echo "$FILE does not exists."
  exit 0;
fi

prologue;

make -f $FILE CPU=$1 ENDIAN=${ENDIAN}

cp ./build/a.out .

epilogue;

