Work on llvm 088f33605d8a61ff519c580a71b1dd57d16a03f8 of 15.x
and f28c006a5895fc0e329fe15fead81e37457cb1d1 of 14.x.

install:
  ~/git/lbt/exlbt/clang-tools$ cp -rf loop-convert2 ~/llvm/15.x/llvm-project/clang-tools-extra/.
  echo 'add_subdirectory(loop-convert2)' >> ~/llvm/15.x/llvm-project/clang-tools-extra/CMakeLists.txt

build:
  ~/llvm/14.x/llvm-project/build$ ninja
run:
  ~/git/lbt/exlbt/clang-tools$ ~/llvm/15.x/llvm-project/build/bin/loop-convert2 loop-test.cpp --
  Potential array-based loop discovered.
  ForStmt begin at 8:3
  ForStmt end at 10:3
  ~/git/lbt/exlbt/clang-tools$ ~/llvm/15.x/llvm-project/build/bin/loop-convert2 -debug loop-test.cpp --
  Potential array-based loop discovered.
  ForStmt 0x1378c7018
  |-DeclStmt 0x1378c6dc0
  | `-VarDecl 0x1378c6d38  used i 'int' cinit
  |   `-IntegerLiteral 0x1378c6da0 'int' 0
  |-<<<NULL>>>
  |-BinaryOperator 0x1378c6ed8 '_Bool' '<'
  | |-ImplicitCastExpr 0x1378c6ec0 'int' <LValueToRValue>
  | | `-DeclRefExpr 0x1378c6dd8 'int' lvalue Var 0x1378c6d38 'i' 'int'
  | `-CallExpr 0x1378c6ea0 'int'
  |   `-ImplicitCastExpr 0x1378c6e88 'int (*)(void)' <FunctionToPointerDecay>
  |     `-DeclRefExpr 0x1378c6e40 'int (void)' lvalue Function 0x1378c6a40 'expr' 'int (void)'
  |-UnaryOperator 0x1378c6f18 'int' lvalue prefix '++'
  | `-DeclRefExpr 0x1378c6ef8 'int' lvalue Var 0x1378c6d38 'i' 'int'
  `-CompoundStmt 0x1378c7000
    `-BinaryOperator 0x1378c6fe0 'int' lvalue '='
      |-DeclRefExpr 0x1378c6f30 'int' lvalue Var 0x1378c6c80 'res' 'int'
      `-BinaryOperator 0x1378c6fc0 'int' '+'
        |-ImplicitCastExpr 0x1378c6f90 'int' <LValueToRValue>
        | `-DeclRefExpr 0x1378c6f50 'int' lvalue Var 0x1378c6c80 'res' 'int'
        `-ImplicitCastExpr 0x1378c6fa8 'int' <LValueToRValue>
          `-DeclRefExpr 0x1378c6f70 'int' lvalue Var 0x1378c6d38 'i' 'int'
  ForStmt begin at 8:3
  ForStmt end at 10:3
