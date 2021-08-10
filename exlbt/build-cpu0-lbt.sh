#!/usr/bin/env bash

LLVM_DIR=~/llvm
LLVM_RELEASE_DIR=${LLVM_DIR}/release
LLVM_TEST_DIR=${LLVM_DIR}/test

if test -d ${LLVM_TEST_DIR}; then
  pushd ${LLVM_TEST_DIR}
  ln -s ../llvm-project/lld lld
  cp -rf exlbt/lld/* ${LLVM_TEST_DIR}/lld/.
  cp -rf exlbt/llvm-objdump/* ${LLVM_TEST_DIR}/llvm/tools/llvm-objdump/.
  cp -rf exlbt/elf2hex ${LLVM_TEST_DIR}/llvm/tools/.
  OS=`uname -s`
  echo "OS =" ${OS}
  mkdir build
  cd build
  cmake -DCMAKE_BUILD_TYPE=Debug -DLLVM_TARGETS_TO_BUILD=Cpu0 -DLLVM_ENABLE_PROJECTS="clang;lld" -G "Unix Makefiles" ../llvm
  make -j4
  popd
else
  echo "${LLVM_TEST_DIR} not existed"
  exit 1
fi

