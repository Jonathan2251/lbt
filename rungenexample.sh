#!/usr/bin/env bash

bash ./clean.sh
rm -rf exlbt.tar.gz
CURR_DIR=$(pwd)
echo ${CURR_DIR}
tar -zcvf exlbt.tar.gz exlbt
pushd exlbt
popd

