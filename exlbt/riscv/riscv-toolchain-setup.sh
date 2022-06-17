#!/usr/bin/env bash

GNU_SRC_DIR=$HOME/riscv/git
LLVM_SRC_DIR=$HOME/riscv/git
GNU_NEWLIB_INSTALL_DIR=$HOME/riscv/riscv_newlib
LLVM_NEWLIB_BUILD_DIR=$LLVM_SRC_DIR/llvm-project/build_riscv_newlib

GNU_LINUX_INSTALL_DIR=$HOME/riscv/riscv_linux
LLVM_LINUX_BUILD_DIR=$LLVM_SRC_DIR/llvm-project/build_riscv_linux


get_llvm() {
  pushd $LLVM_SRC_DIR
  git clone https://github.com/llvm/llvm-project.git
  cd llvm-project
  git checkout -b 13.x origin/release/13.x
  mkdir build_riscv_newlib
  mkdir build_riscv_linux
  popd
}

check() {
  if [ ! -d "$GNU_SRC_DIR" ]; then
    echo "GNU_SRC_DIR: $GNU_SRC_DIR not exist"
    exit 1
  fi
  if [ -d "$GNU_NEWLIB_INSTALL_DIR" ]; then
    echo "GNU_NEWLIB_INSTALL_DIR: $GNU_NEWLIB_INSTALL_DIR exist. Remove it before running."
    exit 1
  fi
  if [ -d "$GNU_LINUX_INSTALL_DIR" ]; then
    echo "GNU_LINUX_INSTALL_DIR: $GNU_LINUX_INSTALL_DIR exist. Remove it before running."
    exit 1
  fi
  if [ ! -d "$LLVM_NEWLIB_BUILD_DIR" ]; then
    echo "LLVM_NEWLIB_BUILD_DIR: $LLVM_NEWLIB_BUILD_DIR not exist. Create it before running."
    exit 1
  fi
  if [ ! -d "$LLVM_LINUX_BUILD_DIR" ]; then
    echo "LLVM_LINUX_BUILD_DIR: $LLVM_LINUX_BUILD_DIR not exist. Create it before running."
    exit 1
  fi
}

build_gnu_toolchain() {
  pushd $GNU_SRC_DIR
  git clone https://github.com/riscv/riscv-gnu-toolchain
  cd riscv-gnu-toolchain
  git checkout -b rvv-intrinsic origin/rvv-intrinsic
  mkdir build_newlib
  cd build_newlib
  ../configure --prefix=$GNU_NEWLIB_INSTALL_DIR --with-multilib-generator="rv32i-ilp32--;rv32imafd-ilp32--;rv64ima-lp64--"
  make

  cd ..
  mkdir build_linux
  cd build_linux
  ../configure --prefix=$GNU_LINUX_INSTALL_DIR
  make linux
  popd
}

# DEFAULT_SYSROOT: https://stackoverflow.com/questions/66357013/compile-clang-with-alternative-sysroot
# LLVM_DEFAULT_TARGET_TRIPLE:  https://clang.llvm.org/docs/CrossCompilation.html#general-cross-compilation-options-in-clang
# Use "clang --sysroot" if did not "cmake -DDEFAULT_SYSROOT"
# $GNU_NEWLIB_INSTALL_DIR/clang --gcc-toolchain=$GNU_NEWLIB_INSTALL_DIR --sysroot=$GNU_NEWLIB_INSTALL_DIR/sysroot/ --static test.c
build_llvm_toolchain() {
  pushd $LLVM_NEWLIB_BUILD_DIR
  cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Debug -DLLVM_TARGETS_TO_BUILD="RISCV" -DLLVM_ENABLE_PROJECTS="clang;lld"  -DLLVM_BINUTILS_INCDIR=$GNU_SRC_DIR/riscv-gnu-toolchain/riscv-binutils/include  -DCMAKE_INSTALL_PREFIX=$GNU_NEWLIB_INSTALL_DIR -DLLVM_PARALLEL_COMPILE_JOBS=4  -DLLVM_PARALLEL_LINK_JOBS=1 -DLLVM_DEFAULT_TARGET_TRIPLE=riscv64-unknown-elf -DDEFAULT_SYSROOT=$GNU_NEWLIB_INSTALL_DIR/riscv64-unknown-elf -DLLVM_INSTALL_UTILS=ON ../llvm
  ninja
  ninja install
  popd
  pushd $LLVM_LINUX_BUILD_DIR
  cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Debug -DLLVM_TARGETS_TO_BUILD="RISCV" -DLLVM_ENABLE_PROJECTS="clang;lld"  -DLLVM_BINUTILS_INCDIR=$GNU_SRC_DIR/riscv-gnu-toolchain/riscv-binutils/include  -DCMAKE_INSTALL_PREFIX=$GNU_LINUX_INSTALL_DIR -DLLVM_PARALLEL_COMPILE_JOBS=4  -DLLVM_PARALLEL_LINK_JOBS=1 -DLLVM_DEFAULT_TARGET_TRIPLE=riscv64-unknown-linux-gnu -DDEFAULT_SYSROOT=$GNU_LINUX_INSTALL_DIR/sysroot -DLLVM_INSTALL_UTILS=ON ../llvm
  ninja
  ninja install
  popd
}

get_llvm;
check;
build_gnu_toolchain;
build_llvm_toolchain;
