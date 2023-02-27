Clang example:
  - Converter from cpp to cpp
  - Work on llvm 088f33605d8a61ff519c580a71b1dd57d16a03f8 of 15.x.

install:
  % brew install llvm
build:
  exe/macOS% make clean; make
run:
  ./build/Rewriter ../input.cpp
check:
  cat output.cpp
gdb:
  exe/macOS% gdb --args ./build/Rewriter ../input.cpp 
  (gdb) b MyASTConsumer.cpp:61
  (gdb) r
  61	    TheRewriter.ReplaceText(CallerSR, StringRef(sBuiltinFunc));
