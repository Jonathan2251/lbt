.. _sec-clang-tools:

Appendix B: Clang-tools
=======================

.. contents::
   :local:
   :depth: 4

This chapter shows the clang tools.


Install llvm-14.x with clang tools
----------------------------------

.. rubric:: exlbt/clang-tools/install-llvm14.sh
.. literalinclude:: ../exlbt/clang-tools/install-llvm14.sh

.. code-block:: console

  $ pwd
  $ $HOME/lbt/exlbt/clang-tools
  $ bash install-llvm14.sh

clang-query
-----------

  References [#ASTMatchers]_ [#ASTMatchersRef]_ [#narrowing-matchers]_.

.. code-block:: console

  $ pwd
  $ $HOME/lbt/exlbt/clang-tools
  $ $HOME/git/lbt/exlbt/clang-tools$ ~/llvm/14.x/llvm-project/build/bin/clang-query test1.cpp --
  $ clang-query> m varDecl()
  Match #1:
  
  /home/cschen/git/lbt/exlbt/clang-tools/test1.cpp:1:12: note: "root" binds here
  int addTwo(int num) 
             ^~~~~~~
  
  Match #2:
  
  /home/cschen/git/lbt/exlbt/clang-tools/test1.cpp:6:10: note: "root" binds here
  int main(int, char**) 
           ^~~
  
  Match #3:
  
  /home/cschen/git/lbt/exlbt/clang-tools/test1.cpp:6:15: note: "root" binds here
  int main(int, char**) 
                ^~~~~~
  3 matches.

  $ ~/llvm/14.x/llvm-project/build/bin/clang++ vector-dsl.cpp -emit-llvm -S -Xclang -ast-dump
    ...
    |-CXXOperatorCallExpr 0x558770871e60 <line:112:3, col:17> 'UVec32' lvalue '='
    | |-ImplicitCastExpr 0x558770871e48 <col:5> 'UVec32 &(*)(const UVec32 &) noexcept' <FunctionToPointerDecay>
    | | `-DeclRefExpr 0x5587708716d0 <col:5> 'UVec32 &(const UVec32 &) noexcept' lvalue CXXMethod 0x5587708713d8 'operator=' 'UVec32 &(const UVec32 &) noexcept'
    | |-DeclRefExpr 0x5587708711c0 <col:3> 'UVec32' lvalue Var 0x558770870900 'A' 'UVec32'
    | `-ImplicitCastExpr 0x5587708716b8 <col:7, col:17> 'const UVec32' lvalue <NoOp>
    |   `-CXXOperatorCallExpr 0x5587708713a0 <col:7, col:17> 'UVec32' lvalue '+'
    |     |-ImplicitCastExpr 0x558770871388 <col:15> 'UVec32 &(*)(const UVec32 &)' <FunctionToPointerDecay>
    |     | `-DeclRefExpr 0x558770871340 <col:15> 'UVec32 &(const UVec32 &)' lvalue CXXMethod 0x558770869960 'operator+' 'UVec32 &(const UVec32 &)'
    |     |-CXXOperatorCallExpr 0x5587708712d0 <col:7, col:13> 'UVec32' lvalue '*'
    |     | |-ImplicitCastExpr 0x5587708712b8 <col:12> 'UVec32 &(*)(const uint32_t, UVec32 &)' <FunctionToPointerDecay>
    |     | | `-DeclRefExpr 0x558770871238 <col:12> 'UVec32 &(const uint32_t, UVec32 &)' lvalue Function 0x55877086b740 'operator*' 'UVec32 &(const uint32_t, UVec32 &)'
    |     | |-ImplicitCastExpr 0x558770871220 <col:7> 'uint32_t':'unsigned int' <LValueToRValue>
    |     | | `-DeclRefExpr 0x5587708711e0 <col:7> 'uint32_t':'unsigned int' lvalue Var 0x5587708710d0 'alpha' 'uint32_t':'unsigned int'
    |     | `-DeclRefExpr 0x558770871200 <col:13> 'UVec32' lvalue Var 0x558770870b88 'B' 'UVec32'
    |     `-ImplicitCastExpr 0x558770871328 <col:17> 'const UVec32' lvalue <NoOp>
    |       `-DeclRefExpr 0x558770871308 <col:17> 'UVec32' lvalue Var 0x558770870e10 'C' 'UVec32'
    ...
  $ ~/llvm/14.x/llvm-project/build/bin/clang-query vector-dsl.cpp --
  $ clang-query> m cxxOperatorCallExpr()
  
  Match #1:
  
  /home/cschen/git/lbt/exlbt/clang-tools/vector-dsl.cpp:95:10: note: "root" binds here
    return B * Scalar;
           ^~~~~~~~~~

  Match #2:
  
  /home/cschen/git/lbt/exlbt/clang-tools/vector-dsl.cpp:113:3: note: "root" binds here
    A = alpha*B + C;
    ^~~~~~~~~~~~~~~
  
  Match #3:
  
  /home/cschen/git/lbt/exlbt/clang-tools/vector-dsl.cpp:113:7: note: "root" binds here
    A = alpha*B + C;
        ^~~~~~~~~~~
  
  Match #4:
  
  /home/cschen/git/lbt/exlbt/clang-tools/vector-dsl.cpp:113:7: note: "root" binds here
    A = alpha*B + C;
        ^~~~~~~
  4 matches.


.. [#ASTMatchers] https://clang.llvm.org/docs/LibASTMatchersTutorial.html

.. [#ASTMatchersRef] https://clang.llvm.org/docs/LibASTMatchersReference.html

.. [#narrowing-matchers] https://clang.llvm.org/docs/LibASTMatchersReference.html#narrowing-matchers
