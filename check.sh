#!/usr/bin/env bash

pushd exlbt/input
bash clean.sh
echo "bash make.sh cpu032I eb Makefile.slinker"
bash make.sh cpu032I eb Makefile.slinker
if [ "$?" != "0" ]; then
  echo "FAIL"
  exit 1;
fi
pushd ~/git/lbd/lbdex/verilog
make
./cpu0Is
popd

echo "bash make.sh cpu032II el Makefile.float"
bash make.sh cpu032II el Makefile.float
if [ "$?" != "0" ]; then
  echo "FAIL"
  exit 1;
fi
pushd ~/git/lbd/lbdex/verilog
./cpu0IIs
popd

echo "bash make.sh cpu032I el Makefile.builtins"
bash make.sh cpu032I el Makefile.builtins
if [ "$?" != "0" ]; then
  echo "FAIL"
  exit 1;
fi
pushd ~/git/lbd/lbdex/verilog
./cpu0Is
popd

echo "bash make.sh cpu032II el Makefile.ch13_1"
bash make.sh cpu032II el Makefile.ch13_1
if [ "$?" != "0" ]; then
  echo "FAIL"
  exit 1;
fi
pushd ~/git/lbd/lbdex/verilog
./cpu0IIs
popd


echo "PASS"
popd
