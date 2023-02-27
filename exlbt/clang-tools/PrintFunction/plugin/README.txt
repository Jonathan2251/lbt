Clang example:
  - Modified from clang/examples/PrintFunctionNames.
  - Work on llvm 088f33605d8a61ff519c580a71b1dd57d16a03f8 of 15.x.

Install:
  plugin$ cp -rf PrintFunction ~/llvm/15.x/llvm-project/clang/examples/.
  echo 'add_subdirectory(PrintFunction)' >> ~/llvm/15.x/llvm-project/clang/examples/CMakeLists.txt

Run command:
  Check ./PrintFunction/README.txt

Others:
  Change MY_DEBUG in plugin/PrintFunction/MyASTConsumer.h to enable or disable DEBUG messages.
