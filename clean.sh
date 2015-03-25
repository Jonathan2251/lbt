#!/usr/bin/env bash

if test -d lbdex; then
  pushd lbdex
  if test -d input; then
    pushd input
    bash clean.sh
    popd
  fi
  if test -d verilog; then
    pushd verilog
    bash clean.sh
    popd
  fi
  popd
fi

if test -d exlbt; then
  pushd exlbt
  if test -d input; then
    pushd input
    bash clean.sh
    popd
  fi
  if test -d libsoftfloat/compiler-rt; then
    pushd libsoftfloat/compiler-rt
    bash clean.sh
    popd
  fi
  if test -d libc/avr-libc-1.8.1; then
    pushd libc/avr-libc-1.8.1
    bash clean.sh
    popd
  fi
  popd
fi
rm -rf build exlbt.tar.gz
rm -f `find . -name \*~`
rm -f `find . -name .DS_Store`

