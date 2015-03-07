#!/usr/bin/env bash

pushd exlbt
pushd input
bash clean.sh
popd
pushd libsoftfloat/compiler-rt
bash clean.sh
popd
pushd libc/avr-libc-1.8.1
bash clean.sh
popd
popd
rm -rf build exlbt.tar.gz
rm -f `find . -name \*~`
rm -f `find . -name .DS_Store`

