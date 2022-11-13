#!/usr/bin/env bash

# Fail as follows,
# riscv % /usr/local/Cellar/riscv-gnu-toolchain/main/riscv64-unknown-elf/bin/clang++ hello.c -menable-experimental-extensions -march=rv64gcv1p0 -O0 -mabi=lp64d -v 
# hello.c:1:10: fatal error: 'stdio.h' file not found
# #include <stdio.h>

# mkdir riscv/git, riscv/riscv_newlib, riscv_linux before running this bash script
export LLVM_VER=14.x
#export LLVM_VER=13.0.0
export GNU_SRC_DIR=$HOME/riscv/git
export LLVM_SRC_DIR=$HOME/riscv/git/$LLVM_VER

export GNU_NEWLIB_INSTALL_DIR=$HOME/riscv/$LLVM_VER/riscv_newlib
export LLVM_NEWLIB_BUILD_DIR=$LLVM_SRC_DIR/llvm-project/build_riscv_newlib


# https://blog.csdn.net/poena/article/details/118400196
# xcode 14.1 and CTL updated version
install_riscv_gnu_macos() {
  arch --x86_64 /usr/local/bin/brew tap riscv/riscv
  arch --x86_64 /usr/local/bin/brew install riscv-tools
}

build_llvm_toolchain_macos() {
  GNU_NEWLIB_INSTALL_DIR=/usr/local/Cellar/riscv-gnu-toolchain/main
  rm -rf $LLVM_NEWLIB_BUILD_DIR
  mkdir $LLVM_NEWLIB_BUILD_DIR
  pushd $LLVM_NEWLIB_BUILD_DIR
  cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Debug -DLLVM_TARGETS_TO_BUILD="RISCV" \
  -DLLVM_ENABLE_PROJECTS="clang;lld"  \
  -DLLVM_OPTIMIZED_TABLEGEN=On \
  -DCMAKE_INSTALL_PREFIX=$GNU_NEWLIB_INSTALL_DIR -DLLVM_PARALLEL_COMPILE_JOBS=4 \
  -DLLVM_PARALLEL_LINK_JOBS=1 -DLLVM_DEFAULT_TARGET_TRIPLE=riscv64-unknown-elf \
  -DDEFAULT_SYSROOT=$GNU_NEWLIB_INSTALL_DIR/riscv64-unknown-elf \
  -DLLVM_INSTALL_UTILS=ON ../llvm
  ninja
  ninja install
  popd 
}

install_riscv_gnu_macos;
build_llvm_toolchain_macos;
