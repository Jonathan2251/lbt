#!/usr/bin/env bash

PWD=`pwd`
pushd ${PWD}
rm -rf a.out dlconfig ../../lbdex/verilog/dlconfig cpu0.hex *~ *.so *.o *.bc *.s *.S
make -f Makefile.float clean
make -C ../sanitizer_common clean
make -C ../compiler-rt clean
make -C ../newlib/libm clean
popd

