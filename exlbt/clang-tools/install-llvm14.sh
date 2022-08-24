#!/usr/bin/env bash

export LLVM_SRC_DIR=$HOME/llvm/14.x

get_llvm14() {
  if [ ! -d "$LLVM_SRC_DIR" ]; then
    echo "LLVM_SRC_DIR: $LLVM_SRC_DIR not exist"
    exit 1
  fi
  pushd $LLVM_SRC_DIR
  git clone https://github.com/llvm/llvm-project.git
  cd llvm-project
  git checkout -b 14.x origin/release/14.x
  mkdir build_riscv_newlib
  mkdir build_riscv_linux
  popd
}

build_llvm14_toolchain() {
  pushd $LLVM_SRC_DIR
  if [ ! -d "$LLVM_SRC_DIR/build" ]; then
    echo "$LLVM_SRC_DIR/build exist already. Please remove it before run this bash"
    exit 1
  fi
  cmake -DCMAKE_BUILD_TYPE=Debug -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" \
  -DLLVM_PARALLEL_COMPILE_JOBS=4 -DLLVM_PARALLEL_LINK_JOBS=1 -G "Ninja" ../llvm \
  -DCLANG_BUILD_EXAMPLES=ON
  ninja
  ninja install
  popd
}

get_llvm;
build_llvm14_toolchain;
