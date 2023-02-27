This is a simple example demonstrating how to use clang's facility for
providing AST consumers using a plugin.

Once the plugin is built, you can run it using:
--
Linux:
  $ <llvm-build-path>/bin/clang++ -Xclang -load -Xclang <llvm-build-path>/lib/PrintFunction.so -Xclang -add-plugin -Xclang ex1-act -Xclang -emit-llvm -S ../input1.cpp -o -
  for example:
    ~/llvm/15.x/llvm-project/build/bin/clang++ -Xclang -load -Xclang ~/llvm/15.x/llvm-project/build/lib/PrintFunction.so -Xclang -add-plugin -Xclang ex1-act -Xclang -ast-dump -fsyntax-only ../input1.cpp -o -

MacOS:
  Use PrintFunction.dylib instead of .so.

Others:
  Change MY_DEBUG in plugin/PrintFunction/MyASTConsumer.h to enable or disable DEBUG messages.
