#!/usr/bin/env bash

# for example:
# bash make.sh cpu032I be Makefile.newlib
# bash make.sh cpu032II be Makefile.builtins
# bash make.sh cpu032I le Makefile.slinker
# bash make.sh cpu032II be Makefile.float
# bash make.sh cpu032II be Makefile.printf-stdarg-2
# bash make.sh cpu032II be Makefile.ch13_1
# bash make.sh cpu032I le Makefile.sanitizer-printf

NEWLIB_DIR=$HOME/git/newlib-cygwin

ARG_NUM=$#
CPU=$1
ENDIAN=$2
MKFILE=$3

build_newlib() {
  pushd $NEWLIB_DIR
  rm -rf build
  mkdir build
  cd build
  CC=$TOOLDIR/clang \
  CFLAGS="-target cpu0$ENDIAN-unknown-linux-gnu -mcpu=$CPU -static -fintegrated-as -Wno-error=implicit-function-declaration" \
  AS="$TOOLDIR/clang -static -fintegrated-as -c" AR="$TOOLDIR/llvm-ar" RANLIB="$TOOLDIR/llvm-ranlib" \
  READELF="$TOOLDIR/llvm-readelf" ../newlib/configure --host=cpu0
  make
  popd
}

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
  if [ $CPU != cpu032I ] && [ $CPU != cpu032II ]; then
    echo "1st argument is cpu032I or cpu032II"
    exit 1
  fi

  INCDIR=../../lbdex/input
  OS=`uname -s`
  echo "OS =" ${OS}

  TOOLDIR=~/llvm/test/build/bin
  CLANG=~/llvm/test/build/bin/clang

  echo "CPU =" "${CPU}"

  if [ "$ENDIAN" != "" ] && [ $ENDIAN != le ] && [ $ENDIAN != be ]; then
    echo "2nd argument is be (big ENDIAN, default) or le (little ENDIAN)"
    exit 1
  fi
  if [ "$ENDIAN" == "" ] || [ $ENDIAN == be ]; then
    ENDIAN=
  else
    ENDIAN=el
  fi
  echo "ENDIAN =" "${ENDIAN}"

  bash clean.sh
  if [ $MKFILE == "Makefile.newlib" ] || [ $MKFILE == "Makefile.builtins" ]; then
    echo "build_newlib"
    build_newlib;
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

make -f $FILE CPU=${CPU} ENDIAN=${ENDIAN} NEWLIB_DIR=${NEWLIB_DIR}

cp ./build/a.out .

epilogue;

