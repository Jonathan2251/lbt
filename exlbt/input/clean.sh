#!/usr/bin/env bash

PWD=`pwd`
pushd ${PWD}
rm -rf a.out dlconfig ../../lbdex/verilog/dlconfig cpu0.hex *~ *.so *.o *.bc *.s *.S
make -f Makefile.slink clean
make -f Makefile.float clean
make -C ../compiler-rt clean
make -f Makefile.builtins clean
make -f Makefile.newlib clean
popd

