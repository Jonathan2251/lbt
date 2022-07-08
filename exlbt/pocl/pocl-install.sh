#!/usr/bin/env bash
# Set POCL_PARENT_DIR for the parent folder of pocl git hub checkout to.
export POCL_PARENT_DIR=$HOME/git

# After "sudo make install", pocl installed in /usr/local/share/pocl, /usr/local/lib/libpocl.so, /usr/local/lib/pocl and /usr/local/bin/poclcc

# Ubuntu 18.04 only can use LLVM_VERSION 13. 14 is too new and dependence packages of Ubuntu 18.04.
LLVM_VERSION=13

install_dependences() {
  echo "LLVM_VERSION: $LLVM_VERSION"
  sudo apt-get install -y build-essential ocl-icd-libopencl1 cmake git pkg-config libclang-${LLVM_VERSION}-dev clang llvm-${LLVM_VERSION} make ninja-build ocl-icd-libopencl1 ocl-icd-dev ocl-icd-opencl-dev libhwloc-dev zlib1g zlib1g-dev clinfo dialog apt-utils libxml2-dev libclang-cpp${LLVM_VERSION}-dev libclang-cpp${LLVM_VERSION} llvm-${LLVM_VERSION}-dev
}

get_pocl() {
  pushd $POCL_PARENT_DIR
  git clone https://github.com/pocl/pocl
  cd pocl
  git checkout 4627171d40543091e399989c277faa52fcee0ff8
  popd
}

check() {
  if [ ! -d "$POCL_PARENT_DIR" ]; then
    echo "POCL_PARENT_DIR: $POCL_PARENT_DIR not exist"
    exit 1
  fi
}

build_pocl() {
  pushd $POCL_PARENT_DIR/pocl
  mkdir build
  cd build
  cmake -DLLVM_CONFIG=/usr/lib/llvm-13/bin/llvm-config ..
  make
  sudo make install
  popd
}

# http://portablecl.org/docs/html/development.html
check_pocl() {
  pushd $POCL_PARENT_DIR/pocl/build
  make check_tier1
  popd
}

install_dependences;
get_pocl;
check;
build_pocl;
check_pocl;
