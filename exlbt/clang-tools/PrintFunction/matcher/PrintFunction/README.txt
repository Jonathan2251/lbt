Work on llvm 088f33605d8a61ff519c580a71b1dd57d16a03f8 of 15.x

install:
  ~/git/lbt/exlbt/clang-tools$ cp -rf PrintFunction ~/llvm/15.x/llvm-project/clang-tools-extra/.
  echo 'add_subdirectory(PrintFunction)' >> ~/llvm/15.x/llvm-project/clang-tools-extra/CMakeLists.txt

build:
  ~/llvm/15.x/llvm-project/build$ ninja
run:
  ~/git/lbt/exlbt/clang-tools/PrintFunction$ ~/llvm/15.x/llvm-project/build/bin/PrintFunction input1.cpp --
  ...
  ~/git/lbt/exlbt/clang-tools/PrintFunction$ ~/llvm/15.x/llvm-project/build/bin/PrintFunction -debug input1.cpp --
