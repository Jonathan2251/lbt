#!/usr/bin/env bash

pushd lbtex
pushd lbt_input
bash clean.sh
popd
pushd libsoftfloat/compiler-rt
bash clean.sh
popd
pushd libc/avr-libc-1.8.1
bash clean.sh
popd
popd
rm -rf build lbtex.tar.gz
rm -f `find . -name \*~`
rm -f `find . -name .DS_Store`

