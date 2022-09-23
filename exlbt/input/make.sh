#!/usr/bin/env bash

# for example:
# bash make.sh cpu032I eb Makefile.newlib
# bash make.sh cpu032I eb Makefile.builtins
# bash make.sh cpu032I el Makefile.slinker
# bash make.sh cpu032II eb Makefile.float
# bash make.sh cpu032II el Makefile.ch13_1

LBDEX_DIR=$HOME/git/lbd/lbdex
NEWLIB_DIR=$HOME/git/newlib-cygwin

ARG_NUM=$#
CPU=$1
ENDIAN=$2
MKFILE=$3

BUILD_DIR=build-$CPU-$ENDIAN

build_newlib() {
  pushd $NEWLIB_DIR
  rm -rf $BUILD_DIR
  mkdir $BUILD_DIR
  cd build
  CC=$TOOLDIR/clang \
  CFLAGS="-target cpu0$ENDIAN-unknown-linux-gnu -mcpu=$CPU -static \
          -fintegrated-as -Wno-error=implicit-function-declaration" \
          AS="$TOOLDIR/clang -static -fintegrated-as -c" \
          AR="$TOOLDIR/llvm-ar" RANLIB="$TOOLDIR/llvm-ranlib" \
          READELF="$TOOLDIR/llvm-readelf" ../newlib/configure --host=cpu0
  make
  popd
}

prologue() {
  if [ $ARG_NUM == 0 ]; then
    echo "useage: bash $sh_name cpu_type ENDIAN"
    echo "  cpu_type: cpu032I or cpu032II"
    echo "  ENDIAN: be (big ENDIAN, default) or le (little ENDIAN)"
    echo "for example:"
    echo "  bash build-slinker.sh cpu032I eb"
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
  echo "ENDIAN =" "${ENDIAN}"

  if [ $ENDIAN != eb ] && [ $ENDIAN != el ]; then
    echo "2nd argument is be (big ENDIAN, default) or le (little ENDIAN)"
    exit 1
  fi

  if [ $MKFILE == "Makefile.newlib" ] || [ $MKFILE == "Makefile.builtins" ]; then
    echo "build_newlib"
#    build_newlib;
  fi
  rm $BUILD_DIR/a.out
}

isLittleEndian() {
  echo "ENDIAN = " "$ENDIAN"
  if [ "$ENDIAN" == "LittleEndian" ] ; then
    LE="true"
  elif [ "$ENDIAN" == "BigEndian" ] ; then
    LE="false"
  else
    echo "!ENDIAN unknown"
    exit 1
  fi
}

elf2hex() {
  ${TOOLDIR}/elf2hex -le=$LE $BUILD_DIR/a.out > ${LBDEX_DIR}/verilog/cpu0.hex
  if [ $LE == "true" ] ; then
    echo "1   /* 0: big ENDIAN, 1: little ENDIAN */" > ${LBDEX_DIR}/verilog/cpu0.config
  else
    echo "0   /* 0: big ENDIAN, 1: little ENDIAN */" > ${LBDEX_DIR}/verilog/cpu0.config
  fi
  cat ${LBDEX_DIR}/verilog/cpu0.config
}

epilogue() {
  ENDIAN=`${TOOLDIR}/llvm-readobj -h $BUILD_DIR/a.out|grep "DataEncoding"|awk '{print $2}'`
  isLittleEndian;
  elf2hex;
}

FILE=$3

if [ ! -f "$FILE" ]; then
  echo "$FILE does not exists."
  exit 0;
fi

prologue;

make -f $FILE CPU=${CPU} ENDIAN=${ENDIAN} LBDEX_DIR=${LBDEX_DIR} NEWLIB_DIR=${NEWLIB_DIR}

epilogue;

