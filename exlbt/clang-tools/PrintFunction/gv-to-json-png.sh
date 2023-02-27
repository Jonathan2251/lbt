#!/usr/bin/env bash

DIRS="exe/macOS exe/Linux"

for DIR in $DIRS
do
  pushd $DIR
  dot -Tjson output1.gv -o output1.json
  dot -Tjson output2.gv -o output2.json
  dot -Tjson output3.gv -o output3.json

  dot -Tpng output1.gv -o output1.png
  dot -Tpng output2.gv -o output2.png
  dot -Tpng output3.gv -o output3.png
  popd
done
