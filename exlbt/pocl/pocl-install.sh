#!/usr/bin/env bash
# Set POCL_PARENT_DIR for the parent folder of pocl git hub checkout to.
export POCL_PARENT_DIR=$HOME/git

# After "sudo make install", pocl installed in /usr/local/share/pocl, 
# /usr/local/lib/libpocl.so, /usr/local/lib/pocl and /usr/local/bin/poclcc.

# Ubuntu 18.04 only can use LLVM_VERSION 13. 14 is too new for the dependent 
# packages of Ubuntu 18.04.
LLVM_VERSION=13


#LLVM_PATH=/usr/lib/llvm-13/bin
LLVM_PATH=$HOME/llvm/13/llvm-project/build//bin

# Todo:
# Trace test_clCreateKernel.c and test_enqueue_kernel_from_binary.c for running an OpenCL example.

install_dependences() {
  echo "LLVM_VERSION: $LLVM_VERSION"
  sudo apt-get install -y build-essential ocl-icd-libopencl1 cmake git pkg-config \
  libclang-${LLVM_VERSION}-dev clang llvm-${LLVM_VERSION} make ninja-build \
  ocl-icd-libopencl1 ocl-icd-dev ocl-icd-opencl-dev libhwloc-dev zlib1g \
  zlib1g-dev clinfo dialog apt-utils libxml2-dev libclang-cpp${LLVM_VERSION}-dev \
  libclang-cpp${LLVM_VERSION} llvm-${LLVM_VERSION}-dev
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
# The default uses /usr/bin/cc in ubuntu 18.04
  #cmake -WITH_DLLVM_CONFIG=/usr/lib/llvm-13/bin/llvm-config ..
# Have verified the following using clang compiler
  cmake -DWITH_LLVM_CONFIG=${LLVM_PATH}/llvm-config \
  -DENABLE_ICD=OFF \
  -DCMAKE_C_COMPILER=${LLVM_PATH}/clang \
  -DCMAKE_CXX_COMPILER=${LLVM_PATH}/clang++ ..
  make
  sudo make install
  popd
}

# Verify tests/runtime/test_clCreateKernel.c using clang rather than cc as follows,
# jonathanchen@hz-compiler1:~/git/pocl/build$ touch ../tests/runtime/test_clCreateKernel.c
# jonathanchen@hz-compiler1:~/git/pocl/build$ make VERBOSE=1 |grep test_clCreateKernel.c
# [ 95%] Building C object tests/runtime/CMakeFiles/test_clCreateKernel.dir/test_clCreateKernel.c.o
# cd /home/jonathanchen/git/pocl/build/tests/runtime && /usr/lib/llvm-13/bin/clang ...
# -c /home/jonathanchen/git/pocl/tests/runtime/test_clCreateKernel.c

# http://portablecl.org/docs/html/development.html
check_pocl() {
  pushd $POCL_PARENT_DIR/pocl/build
  make check_tier1
  popd
}

#install_dependences;
#get_pocl;
#check;
build_pocl;
#check_pocl;
