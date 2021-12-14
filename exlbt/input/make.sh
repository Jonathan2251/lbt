#!/usr/bin/env bash

# for example:
# bash make.sh cpu032I le Makefile.slinker
# bash make.sh cpu032II be Makefile.float-necessary
# bash make.sh cpu032I be Makefile.slinker.mips-clang

source functions.sh

sh_name=make.sh
argNum=$#
arg1=$1
arg2=$2
FILE=$3

if [ ! -f "$FILE" ]; then
  echo "$FILE does not exists."
  exit 0;
fi

prologue;

make -f $FILE clean
make -f $FILE CPU=$1 endian=${endian}

cp ./build/a.out .

epilogue;

