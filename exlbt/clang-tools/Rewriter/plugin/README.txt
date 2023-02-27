Clang example:
  - Converter from cpp to cpp.
  - Modified from clang/examples/PrintFunctionNames.
  - Work on llvm 088f33605d8a61ff519c580a71b1dd57d16a03f8 of 15.x,
    f28c006a5895fc0e329fe15fead81e37457cb1d1 of 14.x and
    e8a397203c67adbeae04763ce25c6a5ae76af52c of 12.x.

Install:
  ~/git/lbt/exlbt/clang-tools/Rewriter/plugin$ cp -rf Rewriter ~/llvm/15.x/llvm-project/clang/examples/.
  echo 'add_subdirectory(Rewriter)' >> ~/llvm/15.x/llvm-project/clang/examples/CMakeLists.txt

Run command:
  Check Rewriter/README.txt
