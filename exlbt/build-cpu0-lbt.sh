#!/usr/bin/env bash

LLVM_DIR=~/llvm
LLVM_DEBUG_DIR=${LLVM_DIR}/debug
LLVM_TEST_DIR=${LLVM_DIR}/test

if test -d ${LLVM_TEST_DIR}; then
  pushd ${LLVM_TEST_DIR}
  ln -s ../llvm-project/clang clang
  ln -s ../llvm-project/lld lld
  popd
  cp -rf clang/* ${LLVM_TEST_DIR}/clang/.
  cp -rf lld/* ${LLVM_TEST_DIR}/lld/.
  cp -rf llvm-objdump/* ${LLVM_TEST_DIR}/llvm/tools/llvm-objdump/.
  cp -rf elf2hex ${LLVM_TEST_DIR}/llvm/tools/.
  OS=`uname -s`
  echo "OS =" ${OS}
  pushd ${LLVM_TEST_DIR}
  rm -rf build
  mkdir build
  cd build
# clang has better diagnosis in report error message
# LLVM_DEFAULT_TARGET_TRIPLE=cpu0-unknown-elf: for llvm-config --host-target
  cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang \
  -DLLVM_TARGETS_TO_BUILD=Cpu0 -DLLVM_ENABLE_PROJECTS="clang;lld" \
  -DLLVM_OPTIMIZED_TABLEGEN=On \
  -DLLVM_PARALLEL_COMPILE_JOBS=4 -DLLVM_PARALLEL_LINK_JOBS=1 \
  -DLLVM_DEFAULT_TARGET_TRIPLE=cpu0-unknown-linux-gnu -G "Ninja" ../llvm
  time ninja
  popd
else
  echo "${LLVM_TEST_DIR} not existed"
  exit 1
fi

