.. _sec-clang:

Clang
======

.. contents::
   :local:
   :depth: 4

This chapter adds the Cpu0 target to the frontend Clang.


Cpu0 target
-------------

.. rubric:: exlbt/clang/include/clang/lib/Driver/CMakeLists.txt
.. code-block:: console

  ToolChains/Arch/Cpu0.cpp


.. rubric:: exlbt/clang/lib/Driver/ToolChains/CommonArgs.cpp
.. code-block:: console

  #include "Arch/Cpu0.h"
  ...
    case llvm::Triple::cpu0:
    case llvm::Triple::cpu0el: {
      StringRef CPUName;
      StringRef ABIName;
      cpu0::getCpu0CPUAndABI(Args, T, CPUName, ABIName);
      return std::string(CPUName);
    }

.. rubric:: exlbt/clang/lib/Driver/ToolChains/Arch/Cpu0.h
.. literalinclude:: ../exlbt/clang/lib/Driver/ToolChains/Arch/Cpu0.h

.. rubric:: exlbt/clang/lib/Driver/ToolChains/Arch/Cpu0.cpp
.. literalinclude:: ../exlbt/clang/lib/Driver/ToolChains/Arch/Cpu0.cpp


.. rubric:: exlbt/clang/include/clang/lib/Basic/CMakeLists.txt
.. code-block:: console

  Targets/Cpu0.cpp

.. rubric:: exlbt/clang/include/clang/lib/Basic/Targets.cpp
.. code-block:: console

  #include "Targets/Cpu0.h"
  ...
    case llvm::Triple::cpu0:
      switch (os) {
      case llvm::Triple::Linux:
        return new LinuxTargetInfo<Cpu0TargetInfo>(Triple, Opts);
      case llvm::Triple::RTEMS:
        return new RTEMSTargetInfo<Cpu0TargetInfo>(Triple, Opts);
      case llvm::Triple::FreeBSD:
       return new FreeBSDTargetInfo<Cpu0TargetInfo>(Triple, Opts);
      case llvm::Triple::NetBSD:
       return new NetBSDTargetInfo<Cpu0TargetInfo>(Triple, Opts);
      default:
        return new Cpu0TargetInfo(Triple, Opts);
      }
  
    case llvm::Triple::cpu0el:
      switch (os) {
      case llvm::Triple::Linux:
        return new LinuxTargetInfo<Cpu0TargetInfo>(Triple, Opts);
      case llvm::Triple::RTEMS:
        return new RTEMSTargetInfo<Cpu0TargetInfo>(Triple, Opts);
      case llvm::Triple::FreeBSD:
        return new FreeBSDTargetInfo<Cpu0TargetInfo>(Triple, Opts);
      case llvm::Triple::NetBSD:
        return new NetBSDTargetInfo<Cpu0TargetInfo>(Triple, Opts);
      default:
        return new Cpu0TargetInfo(Triple, Opts);
      }

.. rubric:: exlbt/clang/lib/Basic/Targets/Cpu0.h
.. literalinclude:: ../exlbt/clang/lib/Basic/Targets/Cpu0.h

.. rubric:: exlbt/clang/lib/Basic/Targets/Cpu0.cpp
.. literalinclude:: ../exlbt/clang/lib/Basic/Targets/Cpu0.cpp


.. code-block:: console

  chungshu@ChungShudeMacBook-Air input % ~/llvm/test/build/bin/clang --help-hidden|grep cpu0
    -cpu032II               Equivalent to -march=cpu032II
    -cpu032I                Equivalent to -march=cpu032I


Builtin functions
-----------------

A builtin function is a function that may map to one or more hardware instructions
for performance improvement. The Cpu0 backend ports compiler-rt's builtins in the
section "Compiler-rt's Builtins" of the Library chapter
[#cpu0-compiler-rt-builtins]_.

Built-in kernels are kernels that are specific to a particular device and 
provide a mechanism to allow an application developer to leverage special 
hardware that may be present on the device [#builtin-kernel]_.

.. rubric:: exlbt/clang/include/clang/Basic/TargetBuiltins.h
.. code-block:: console

    /// CPU0 builtins
    namespace Cpu0 {
      enum {
          LastTIBuiltin = clang::Builtin::FirstTSBuiltin-1,
  #define BUILTIN(ID, TYPE, ATTRS) BI##ID,
  #include "clang/Basic/BuiltinsCpu0.def"
          LastTSBuiltin
      };
    }


.. rubric:: exlbt/clang/include/clang/Basic/BuiltinsCpu0.def
.. literalinclude:: ../exlbt/clang/include/clang/Basic/BuiltinsCpu0.def

.. code-block:: console

  chungshu@ChungShudeMacBook-Air CodeGen % pwd
  /Users/chungshu/llvm/test/clang/test/CodeGen
  chungshu@ChungShudeMacBook-Air CodeGen % ~/llvm/test/build/bin/llvm-lit builtins-cpu0.c
  ..  
  -- Testing: 1 tests, 1 workers --
    PASS: Clang :: CodeGen/builtins-cpu0.c (1 of 1)

  Testing Time: 0.14s
    Passed: 1


.. [#cpu0-compiler-rt-builtins] http://jonathan2251.github.io/lbt/lib.html#compiler-rt-s-builtins

.. [#builtin-kernel] Chapter 5.4 of book "Heterogeneous computing with OpenCL2.0, 3rd edition"
