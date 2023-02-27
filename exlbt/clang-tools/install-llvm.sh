#!/usr/bin/env bash

#export LLVM_VER=14.x
#export LLVM_VER=15.x
export LLVM_VER=16.x

export LLVM_SRC_DIR=$HOME/llvm/$LLVM_VER

get_llvm() {
  if [ ! -d "$LLVM_SRC_DIR" ]; then
    echo "LLVM_SRC_DIR: $LLVM_SRC_DIR not exist"
    exit 1
  fi
  pushd $LLVM_SRC_DIR
  git clone https://github.com/llvm/llvm-project.git
  cd llvm-project
  git checkout -b $LLVM_VER origin/release/$LLVM_VER
  popd
}

release_build_llvm_toolchain() {
  pushd $LLVM_SRC_DIR/llvm-project
  if [ -d "$LLVM_SRC_DIR/llvm-project/release-build" ]; then
    echo "$LLVM_SRC_DIR/build exist already. Please remove it before run this bash"
    exit 1
  fi
  mkdir release-build
  cd release-build
  cmake -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" \
  -DLLVM_PARALLEL_COMPILE_JOBS=4 -DLLVM_PARALLEL_LINK_JOBS=1 -G "Ninja" ../llvm \
  -DCLANG_BUILD_EXAMPLES=ON
  ninja
  popd
}

debug_build_llvm_toolchain() {
  pushd $LLVM_SRC_DIR/llvm-project
  if [ -d "$LLVM_SRC_DIR/llvm-project/build" ]; then
    echo "$LLVM_SRC_DIR/build exist already. Please remove it before run this bash"
    exit 1
  fi
  mkdir build
  cd build
  cmake -DCMAKE_BUILD_TYPE=Debug -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" \
  -DLLVM_PARALLEL_COMPILE_JOBS=4 -DLLVM_PARALLEL_LINK_JOBS=1 -G "Ninja" ../llvm \
  -DCLANG_BUILD_EXAMPLES=ON
  ninja
  popd
}

get_llvm;
release_build_llvm_toolchain;
debug_build_llvm_toolchain;
