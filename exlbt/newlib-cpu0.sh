#!/usr/bin/env bash

# change this dir for newlib-cygwin
NEWLIB_PARENT_DIR=$HOME/git

NEWLIB_DIR=$NEWLIB_PARENT_DIR/newlib-cygwin
CURR_DIR=`pwd`
CC=$HOME/llvm/test/build/bin/clang
CFLAGS="-target cpu0el-unknown-linux-gnu -static -fintegrated-as -Wno-error=implicit-function-declaration"
AS="$HOME/llvm/test/build/bin/clang -static -fintegrated-as -c"
AR="$HOME/llvm/test/build/bin/llvm-ar"
RANLIB="$HOME/llvm/test/build/bin/llvm-ranlib"
READELF="$HOME/llvm/test/build/bin/llvm-readelf"

install_newlib() {
  pushd $NEWLIB_PARENT_DIR
  git clone git://sourceware.org/git/newlib-cygwin.git
  cd newlib-cygwin
  git checkout dcb25665be227fb5a05497b7178a3d5df88050ec
  cp $CURR_DIR/newlib.patch .
  git apply newlib.patch
  cp -rf $CURR_DIR/newlib-cygwin/newlib/libc/machine/cpu0 newlib/libc/machine/. 
  cp -rf $CURR_DIR/newlib-cygwin/libgloss/cpu0 libgloss/. 
  popd
}

build_cpu0() {
  rm -rf build-$CPU-$ENDIAN
  mkdir build-$CPU-$ENDIAN
  cd build-$CPU-$ENDIAN
  CFLAGS="-target cpu0$ENDIAN-unknown-linux-gnu -mcpu=$CPU -static -fintegrated-as -Wno-error=implicit-function-declaration"
  CC=$CC CFLAGS=$CFLAGS AS=$AS AR=$AR RANLIB=$RANLIB READELF=$READELF ../newlib/configure --host=cpu0
  make
  cd ..
}

build_newlib() {
  pushd $NEWLIB_DIR
  CPU=cpu032I
  ENDIAN=eb
  build_cpu0;
  CPU=cpu032I
  ENDIAN=el
  build_cpu0;
  CPU=cpu032II
  ENDIAN=eb
  build_cpu0;
  CPU=cpu032II
  ENDIAN=el
  build_cpu0;
  popd
}

install_newlib;
build_newlib;
