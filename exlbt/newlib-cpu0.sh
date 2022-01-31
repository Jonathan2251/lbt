#!/usr/bin/env bash

NEWLIB_PARENT_DIR=$HOME/git/2
CURR_DIR=`pwd`

install_newlib() {
  pushd $NEWLIB_PARENT_DIR
  git clone git://sourceware.org/git/newlib-cygwin.git
  cd newlib-cygwin
  git checkout dcb25665be227fb5a05497b7178a3d5df88050ec
  cp $CURR_DIR/newlib.patch .
  git apply newlib.patch
  cp -rf $CURR_DIR/newlib-cygwin/newlib/libc/machine/cpu0 newlib/libc/machine/. 
  popd
}

build_newlib() {
  pushd $NEWLIB_PARENT_DIR/newlib-cygwin
  mkdir build
  cd build
  CC=/Users/cschen/llvm/test/build/bin/clang CFLAGS="-static -fintegrated-as -Wno-error=implicit-function-declaration" AS="/Users/cschen/llvm/test/build/bin/clang -static -fintegrated-as -c" AR="/Users/cschen/llvm/test/build/bin/llvm-ar" RANLIB="/Users/cschen/llvm/test/build/bin/llvm-ranlib" READELF="/Users/cschen/llvm/test/build/bin/llvm-readelf" ../newlib/configure --host=cpu0
  make
  popd
}

install_newlib;
build_newlib;
