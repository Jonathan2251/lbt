Clang example:
  - Work on llvm 088f33605d8a61ff519c580a71b1dd57d16a03f8 of 15.x.

build:
  exe/Linux$ make clean; make
run:
  ./build/PrintFunction ../../input1.cpp
check:
  cat output.cpp
gdb:
  exe/Linux$ gdb --args ./build/PrintFunction ../../input1.cpp 

Others:
  Change MY_DEBUG in plugin/PrintFunction/MyASTConsumer.h to enable or disable DEBUG messages.
