#!/usr/bin/env bash

bash ./clean.sh
rm -rf lbtex.tar.gz
CURR_DIR=$(pwd)
echo ${CURR_DIR}
tar -zcvf lbtex.tar.gz lbtex
pushd lbtex
popd

