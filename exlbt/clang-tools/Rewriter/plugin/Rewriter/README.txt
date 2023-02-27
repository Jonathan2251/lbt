This is a simple example demonstrating how to use clang's facility for
providing AST consumers using a plugin.

Once the plugin is built, you can run it using:
--
Linux:
  $ <llvm-build-path>/bin/clang++ -Xclang -load -Xclang <llvm-build-path>/lib/Rewriter.so -Xclang -add-plugin -Xclang ex1-act -Xclang -emit-llvm -S input.cpp -o -
  for example:
    ~/llvm/15.x/llvm-project/build/bin/clang++ -Xclang -load -Xclang ~/llvm/15.x/llvm-project/build/lib/Rewriter.so -Xclang -add-plugin -Xclang ex1-act -Xclang -ast-dump -fsyntax-only input.cpp -o -

MacOS:
  Use Rewriter.dylib instead of .so.
