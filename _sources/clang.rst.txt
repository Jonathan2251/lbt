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

.. rubric:: exlbt/input/build-float-necessary-2.sh
.. literalinclude:: ../exlbt/input/build-float-necessary-2.sh

Build and run as follows,

.. code-block:: console

  chungshu@ChungShudeMacBook-Air input % bash build-slinker-2.sh cpu032I le                                                                
  ...
  endian =  LittleEndian
  ...
  chungshu@ChungShudeMacBook-Air verilog % iverilog -o cpu0Is cpu0Is.v 
  chungshu@ChungShudeMacBook-Air verilog % ./cpu0Is
  ...

  Harware interrupt 1
  total cpu cycles = 248047              
  RET to PC < 0, finished!
  chungshu@ChungShudeMacBook-Air input % bash build-slinker-2.sh cpu032II be
  ...
  endian =  BigEndian
  chungshu@ChungShudeMacBook-Air verilog % iverilog -o cpu0IIs cpu0IIs.v 
  chungshu@ChungShudeMacBook-Air verilog % ./cpu0IIs
  ...
  Harware interrupt 1
  total cpu cycles = 228527              
  RET to PC < 0, finished!
  chungshu@ChungShudeMacBook-Air input % bash build-float-necessary-2.sh cpu032II be 
  chungshu@ChungShudeMacBook-Air verilog % ./cpu0IIs
  WARNING: cpu0.v:487: $readmemh(cpu0.hex): Not enough words in the file for the requested range [0:524287].
  taskInterrupt(001)
  (int)test_shift_left<long long>(0x12, 4) = 288
  (int)test_shift_right<long long>(0x001666660000000a, 48) = 22
  (int)test_shift_right<unsigned long long>(0x001666660000000a, 48) = 22
  (int)test_add<float, float, float>(-2.2, 3.3) = 1
  (int)test_mul<float, float, float>(-2.2, 3.3) = -7
  (int)test_div<float, float, float>(-1.8, 0.5) = -3
  (int)test_add<double, double, float>(-2.2, 3.3) = 1
  (int)test_mul<double, float, double>(-2.2, 3.3) = -7
  (int)test_mul<float, float, double>(-2.2, 3.3) = 0
  (int)test_div<double, double, double>(-1.8, 0.5) = -3
  total cpu cycles = 183630              
  RET to PC < 0, finished!
