.. _sec-clang-tools:

Appendix B: Clang library and tools
===================================

.. contents::
   :local:
   :depth: 4

This chapter shows how to use Clang as a Library and Tools [clang-doc]_.


Install LLVM with Clang Tools
-----------------------------

.. rubric:: exlbt/clang-tools/install-llvm.sh
.. literalinclude:: ../exlbt/clang-tools/install-llvm.sh

.. code-block:: console

  $ pwd
  $ $HOME/lbt/exlbt/clang-tools
  $ bash install-llvm.sh


Using Clang as a Library
------------------------

Clang Plugins
~~~~~~~~~~~~~

Refer example code, clang/examples/PrintFunctionNames [clang-plugins]_ [clang-ast-fa]_.

.. rubric:: exlbt/clang-tools/Rewriter/plugin/README.txt
.. literalinclude:: ../exlbt/clang-tools/Rewriter/plugin/README.txt

.. rubric:: exlbt/clang-tools/Rewriter/plugin/Rewriter/README.txt
.. literalinclude:: ../exlbt/clang-tools/Rewriter/plugin/Rewriter/README.txt


Rewriter/exe/Linux and Rewriter/exe/macOS apply on Linux and macOS as follows,
respectively:

.. rubric:: exlbt/clang-tools/Rewriter/exe/Linux/README.txt
.. literalinclude:: ../exlbt/clang-tools/Rewriter/exe/Linux/README.txt

.. rubric:: exlbt/clang-tools/Rewriter/exe/macOS/README.txt
.. literalinclude:: ../exlbt/clang-tools/Rewriter/exe/macOS/README.txt


Using LibTooling and LibASTMatchers
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Refer to `clang-tools-extra/loop-convert` for examples and usage:
[clang-lamt]_ and [clang-lam]_.

.. rubric:: exlbt/clang-tools/loop-convert2/README.txt
.. literalinclude:: ../exlbt/clang-tools/loop-convert2/README.txt


Using Clang as Tools
--------------------

clang-query
~~~~~~~~~~~

References: [#ASTMatchers]_, [#ASTMatchersRef]_, [#narrowing-matchers]_.

RVV reference: ~/riscv/riscv_newlib/lib/clang/13.0.1/include/riscv_vector.h.

.. rubric:: exlbt/clang-tools/vecotor-dsl.cpp
.. literalinclude:: ../exlbt/clang-tools/vector-dsl.cpp

References: [#check-command]_.

.. [clang-doc] https://clang.llvm.org/docs/index.html

.. [clang-plugins] https://clang.llvm.org/docs/ClangPlugins.html

.. [clang-ast-fa] https://clang.llvm.org/docs/RAVFrontendAction.html 

.. [clang-lamt] https://clang.llvm.org/docs/LibASTMatchersTutorial.html

.. [clang-lam] https://clang.llvm.org/docs/LibASTMatchers.html

.. [#ASTMatchers] https://clang.llvm.org/docs/LibASTMatchersTutorial.html

.. [#ASTMatchersRef] https://clang.llvm.org/docs/LibASTMatchersReference.html

.. [#narrowing-matchers] https://clang.llvm.org/docs/LibASTMatchersReference.html#narrowing-matchers

.. [#check-command] https://www.youtube.com/watch?v=-iOtb6luK1Q
