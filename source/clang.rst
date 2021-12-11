.. _sec-clang:

Clang
======

.. contents::
   :local:
   :depth: 4

This chapter add Cpu0 target to frontend clang. 



Cpu0 target
-------------

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


Verify
-------

.. rubric:: exlbt/input/build-slinker-2.sh
.. literalinclude:: ../exlbt/input/build-slinker-2.sh

Build and run as follows,

.. code-block:: console

  JonathantekiiMac:input Jonathan$ bash build-slinker-2.sh cpu032I le
  ...
  endian =  LittleEndian
  JonathantekiiMac:input Jonathan$ iverilog -o cpu0Is cpu0Is.v 
  114-43-184-210:verilog Jonathan$ ./cpu0Is
  ...

  JonathantekiiMac:input Jonathan$ bash build-slinker-2.sh cpu032II be
  ...
  endian =  BigEndian
  JonathantekiiMac:input Jonathan$ iverilog -o cpu0IIs cpu0IIs.v 
  114-43-184-210:verilog Jonathan$ ./cpu0IIs
  ...

