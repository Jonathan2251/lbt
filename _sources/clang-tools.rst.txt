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

References [#ASTMatchers]_ [#ASTMatchersRef]_ [#narrowing-matchers]_. RVV 
reference ~/riscv/riscv_newlib/lib/clang/13.0.1/include/riscv_vector.h.

.. rubric:: exlbt/clang-tools/vecotor-dsl.cpp
.. literalinclude:: ../exlbt/clang-tools/vector-dsl.cpp

- References [#check-command]_ .

.. [#ASTMatchers] https://clang.llvm.org/docs/LibASTMatchersTutorial.html

.. [#ASTMatchersRef] https://clang.llvm.org/docs/LibASTMatchersReference.html

.. [#narrowing-matchers] https://clang.llvm.org/docs/LibASTMatchersReference.html#narrowing-matchers

.. [#check-command] https://www.youtube.com/watch?v=-iOtb6luK1Q
