#!/usr/bin/env bash

LLVM_DIR=~/llvm
LLVM_RELEASE_DIR=${LLVM_DIR}/release
LLVM_TEST_DIR=${LLVM_DIR}/test

if test -d ${LLVM_TEST_DIR}; then
  pushd ${LLVM_TEST_DIR}
  ln -s ../llvm-project/lld lld
  popd
  cp -rf lld/* ${LLVM_TEST_DIR}/lld/.
  cp -rf llvm-objdump/* ${LLVM_TEST_DIR}/llvm/tools/llvm-objdump/.
  cp -rf elf2hex ${LLVM_TEST_DIR}/llvm/tools/.
  OS=`uname -s`
  echo "OS =" ${OS}
  pushd ${LLVM_TEST_DIR}
  mkdir build
  cd build
  cmake -DCMAKE_BUILD_TYPE=Debug -DLLVM_TARGETS_TO_BUILD=Cpu0 -DLLVM_ENABLE_PROJECTS="clang;lld" -DLLVM_PARALLEL_COMPILE_JOBS=4 -DLLVM_PARALLEL_LINK_JOBS=1 -G "Ninja" ../llvm
  ninja
  popd
else
  echo "${LLVM_TEST_DIR} not existed"
  exit 1
fi
