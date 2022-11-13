#!/usr/bin/env bash

# Reference: https://risc-v-getting-started-guide.readthedocs.io/en/latest/linux-qemu.html


export INSTALL_DIR=/local/opt/riscv64-linux
export SRC_DIR=$HOME/riscv/git
export RISCV64_LINUX_DIR=$SRC_DIR/riscv64-linux
export nproc=4

install_prerequisite() {
  sudo apt install autoconf automake autotools-dev curl libmpc-dev libmpfr-dev libgmp-dev \
                   gawk build-essential bison flex texinfo gperf libtool patchutils bc \
                   zlib1g-dev libexpat-dev git
}

get_packages() {
  mkdir $RISCV64_LINUX_DIR
  pushd $RISCV64_LINUX_DIR
  cd riscv64-linux
  git clone https://github.com/qemu/qemu
  git clone https://github.com/torvalds/linux
  git clone https://git.busybox.net/busybox
  popd
}

build_qemu() {
  pushd $RISCV64_LINUX_DIR/qemu
  git checkout v5.0.0
  ./configure --target-list=riscv64-softmmu --prefix=$INSTALL_DIR
  make -j $(nproc)
  make install
  popd
}

build_linux() {
  pushd $RISCV64_LINUX_DIR/linux
#  git checkout v5.4.0
  make ARCH=riscv CROSS_COMPILE=riscv64-unknown-linux-gnu- defconfig
  make ARCH=riscv CROSS_COMPILE=riscv64-unknown-linux-gnu- -j $(nproc)
  popd
}

build_busybox() {
  pushd $RISCV64_LINUX_DIR/busybox
  CROSS_COMPILE=riscv64-unknown-linux-gnu- make defconfig
  CROSS_COMPILE=riscv64-unknown-linux-gnu- make -j $(nproc)
  popd
}

install_prerequisite;
get_packages;
build_qemu;
build_linux;
build_busybox;
