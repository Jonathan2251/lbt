#!/bin/bash -xe

export BINUTILS_SRC=/home/cschen/test/llvm-arm-toolchain/1/binutils-2.23.2
export BINUTILS_INSTALL=/home/cschen/test/llvm-arm-toolchain/1/binutils-2.23.2-install
export MVLTOOLCHAIN=/home/cschen/test/llvm-arm-toolchain/1/Marvell_toolchain_2013_04_12/arm-marvell-linux-gnueabi-hard_x86_64
export SYSROOT=${MVLTOOLCHAIN}/arm-marvell-linux-gnueabi
export BASE=`pwd`
export LLVM_SRC=${BASE}/llvm
export POLLY_SRC=${LLVM_SRC}/tools/polly
export CLANG_SRC=${LLVM_SRC}/tools/clang
export CLOOG_SRC=${BASE}/cloog_src
export CLOOG_INSTALL=${BASE}/cloog_install
export LLVM_BUILD=${BASE}/llvm_arm_build

if [ -e /proc/cpuinfo ]; then
    procs=`cat /proc/cpuinfo | grep processor | wc -l`
else
    procs=1
fi

if ! test -d ${LLVM_SRC}; then
    git clone http://llvm.org/git/llvm.git ${LLVM_SRC}
    cd ${LLVM_SRC}
    git checkout release_34
    cd ..
fi

if ! test -d ${POLLY_SRC}; then
    git clone http://llvm.org/git/polly.git ${POLLY_SRC}
    cd ${POLLY_SRC}
    git checkout release_34
    cd ..
fi

if ! test -d ${CLANG_SRC}; then
    git clone http://llvm.org/git/clang.git ${CLANG_SRC}
    cd ${CLANG_SRC}
    git checkout release_34
    cd ..
fi

${POLLY_SRC}/utils/checkout_cloog.sh ${CLOOG_SRC}
cd ${CLOOG_SRC}

if ! test -e ${CLOOG_SRC}/config.log; then
    ./configure --prefix=${CLOOG_INSTALL}
fi
make
make install
cd ${BASE}

mkdir -p ${LLVM_BUILD}
cd ${LLVM_BUILD}

if which cmake ; then
    cmake -DCMAKE_PREFIX_PATH=${CLOOG_INSTALL} -DCMAKE_CXX_FLAGS=-std=c++11 -DCMAKE_BUILD_TYPE=Debug -DLLVM_DEFAULT_TARGET_TRIPLE=arm-marvell-linux-gnueabi -DLLVM_TARGET_ARCH=arm -DLLVM_ENABLE_THREADS=OFF -DLLVM_BINUTILS_INCDIR=${BINUTILS_SRC}/include -DDEFAULT_SYSROOT=${SYSROOT}/libc -G "Unix Makefiles" ${LLVM_SRC}
    make -j$procs -l$procs
    make check-polly
else
    ${LLVM_SRC}/configure --with-cloog=${CLOOG_INSTALL} --with-isl=${CLOOG_INSTALL}
    make -j$procs -l$procs
    make check-polly -C tools/polly/test/
fi

cp ${BINUTILS_INSTALL}/arm-linux-gnueabi/bin/* ${LLVM_BUILD}/bin/.
cp -rf ${MVLTOOLCHAIN}/lib/gcc ${LLVM_BUILD}/lib/.
if ! test -e ${LLVM_BUILD}/arm-marvell-linux-gnueabi; then
    mkdir ${LLVM_BUILD}/arm-marvell-linux-gnueabi
fi
cp -rf ${SYSROOT}/include ${LLVM_BUILD}/arm-marvell-linux-gnueabi/.

