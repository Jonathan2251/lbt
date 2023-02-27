#!/usr/bin/env bash
# Currently, this script complete build on both riscv_newlib and riscv_linux.
# However, qemu does not create qemu-riscv64 for baremetal on macos. So cannot
# verify it yet.

# Verified on ubuntu 18.04
# mkdir riscv/git, riscv/riscv_newlib, riscv_linux before running this bash script
export LLVM_VER=14.x
#export LLVM_VER=13.0.0
export GNU_SRC_DIR=$HOME/riscv/git
export LLVM_SRC_DIR=$HOME/riscv/git/$LLVM_VER

export GNU_NEWLIB_INSTALL_DIR=$HOME/riscv/$LLVM_VER/riscv_newlib
export LLVM_NEWLIB_BUILD_DIR=$LLVM_SRC_DIR/llvm-project/build_riscv_newlib

export GNU_LINUX_INSTALL_DIR=$HOME/riscv/$LLVM_VER/riscv_linux
export LLVM_LINUX_BUILD_DIR=$LLVM_SRC_DIR/llvm-project/build_riscv_linux

riscv_gnu_toolchain_prerequisites() {
  brew install python3 gawk gnu-sed gmp mpfr libmpc isl zlib expat
# Do the following echo and touch first time
#  echo 'export PATH="/usr/local/opt/expat/bin:$PATH"' >> ~/.zshrc
#  touch ~/.zshrc
  export LDFLAGS="-L/usr/local/opt/expat/lib"
  export CPPFLAGS="-I/usr/local/opt/expat/include"

  brew tap discoteq/discoteq
  brew install flock

  brew install make bison
  echo 'export PATH="/usr/local/opt/make/libexec/gnubin:$PATH"' >> ~/.zshrc
  ln -s /usr/local/opt/make/libexec/gnubin/make /usr/local/opt/make/libexec/gnubin/gnumake
  echo 'export PATH="/usr/local/opt/bison/bin:$PATH"' >> ~/.zshrc
  export LDFLAGS="-L/usr/local/opt/bison/lib"
}

riscv_llvm_prerequisites() {
  brew install ninja
  brew install cmake
}

get_llvm() {
  if [ ! -d "$GNU_SRC_DIR" ]; then
    echo "GNU_SRC_DIR: $GNU_SRC_DIR not exist"
    exit 1
  fi
  rm -rf $LLVM_SRC_DIR
  mkdir $LLVM_SRC_DIR
  pushd $LLVM_SRC_DIR
  git clone https://github.com/llvm/llvm-project.git
  cd llvm-project
  git checkout -b $LLVM_VER origin/release/$LLVM_VER
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
}

build_gnu_toolchain() {
  pushd $GNU_SRC_DIR
  git clone https://github.com/riscv/riscv-gnu-toolchain
  cd riscv-gnu-toolchain
#  Looks branch change from original/rvv-intrinsic to origin/__archive__
#  git checkout -b rvv-intrinsic origin/rvv-intrinsic
# commit 409b951ba6621f2f115aebddfb15ce2dd78ec24f of master branch is work for vadd.vv of vadd1.c
  mkdir build_newlib
  cd build_newlib
# NX27V is 32-64 bits configurable and has HW float point
  ../configure --prefix=$GNU_NEWLIB_INSTALL_DIR \
  --with-arch=rv64gc --with-abi=lp64d
#  --with-multilib-generator="rv32i-ilp32--;rv32imafd-ilp32--;rv64ima-lp64--"
  make -j4
}

# LLVM_OPTIMIZED_TABLEGEN: Builds a release tablegen that gets used during the LLVM build. This can dramatically speed up debug builds.
# LLVM_INSTALL_TOOLCHAIN_ONLY default is OFF already. Check CmakeCache.txt.
#   https://llvm.org/docs/BuildingADistribution.html?highlight=llvm_install_toolchain_only#difference-between-install-and-install-distribution
# LLVM_BINUTILS_INCDIR:
#   https://stackoverflow.com/questions/45715423/how-to-enable-cfi-in-llvm
#   For lld. https://llvm.org/docs/GoldPlugin.html
#   For llvm version 13.0.0 -DLLVM_BINUTILS_INCDIR will fail on ninja as follows,
#   /home/jonathanchen/riscv/git/13.0.0/llvm-project/llvm/tools/gold/gold-plugin.cpp:38:10: fatal error: plugin-api.h: No such file or directory
#    #include <plugin-api.h>
#             ^~~~~~~~~~~~~~
# -DLLVM_BINUTILS_INCDIR=$GNU_SRC_DIR/riscv-gnu-toolchain/riscv-binutils/include will incurs above fail on 13.x
# DEFAULT_SYSROOT: 
#   https://stackoverflow.com/questions/66357013/compile-clang-with-alternative-sysroot
# LLVM_DEFAULT_TARGET_TRIPLE:  
#   https://clang.llvm.org/docs/CrossCompilation.html#general-cross-compilation-options-in-clang
# LLVM_INSTALL_UTILS:BOOL
#   If enabled, utility binaries like FileCheck and not will be installed to CMAKE_INSTALL_PREFIX.
#   https://llvm.org/docs/CMake.html
# Use "clang --sysroot" if did not "cmake -DDEFAULT_SYSROOT"
# $LLVM_NEWLIB_BUILD_DIR/bin/clang++ --gcc-toolchain=$GNU_NEWLIB_INSTALL_DIR test.cpp -march=rv64g -O0 -mabi=lp64d -v
# $LLVM_LINUX_BUILD_DIR/bin/clang++ --gcc-toolchain=$GNU_LINUX_INSTALL_DIR --sysroot=$GNU_LINUX_INSTALL_DIR/sysroot/ --static test.cpp -march=rv64g -O0 -mabi=lp64d -v
build_llvm_toolchain() {
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

riscv_gnu_toolchain_prerequisites;
riscv_llvm_prerequisites;
get_llvm;
check;
build_gnu_toolchain;
build_llvm_toolchain;
