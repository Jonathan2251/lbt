.. _sec-lib:

Library
========

.. contents::
   :local:
   :depth: 4

Since Cpu0 has not hardware float point instructions, it needs soft float point
library to finish the floating point operation. LLVM compiler-rt project include
the software floating point library implementation, so we choose it as the 
implementation. 

Since compiler-rt uses unix/linux rootfs structure, we fill the gap by porting
avr libc.

The Soft Float Library, compiler-rt/lib/builtins, is under going, it's not finished.
The flow as follows,

.. graphviz:: ../Fig/lib/lib.gv


Compiler-rt
-------------

.. table:: Toolchain components [#toolchain]_

  ==============  ==========================  =============
  Component       LLVM                        GNU [#gnu]_
  ==============  ==========================  =============
  C/C++ Compiler  clang/llvm                  gcc
  Assembler       llvm integrated assembler   as
  Linker          ld.lld                      ld.bfd ld.gold
  Runtime         compiler-rt                 libgcc [#libgcc]_
  Unwinder        libunwind                   libgcc_s
  C++ library     libc++abi, libc++           libsupc++ libstdc++
  Utils           llvm-ar, llvm-objdump etc.  ar, objdump etc.
  C library                  -                libc
  ==============  ==========================  =============

The libgcc's soft float library is here [#soft-float-lib]_ .
Compiler-rt is a project with runtime libraries implentation [#compiler-rt]_ .
Compiler-rt/lib/builtins provides functions for basic operations such as +, -, 
\*, /, ... on type of float or double and for conversion between float and 
integer. Though the 'rt' means RunTime libaraies, most of these functions 
written in target-independent C form and can be compiled and static-linked
into target. When you compile the following c code, llc generates 
**jsub __addsf3** to call compiler-rt float function since Cpu0 hasn't hardware
float-instructions so Cpu0 backend doesn't handle it, and llvm treats it
as a function call for float-add instruction.

.. rubric:: lbt/exlbt/input/ch_call_compilerrt_func.c
.. literalinclude:: ../exlbt/input/ch_call_compilerrt_func.c

.. code-block:: console

  chungshu@ChungShudeMacBook-Air input % clang -target mips-unknown-linux-gnu -S ch_call_compilerrt_func.c -emit-llvm -o ch_call_compilerrt_func.ll
  chungshu@ChungShudeMacBook-Air input % cat ch_call_compilerrt_func.ll
    ...
    %4 = load float, float* %1, align 4
    %5 = load float, float* %2, align 4
    %6 = fadd float %4, %5

  chungshu@ChungShudeMacBook-Air input % ~/llvm/test/build/bin/llc -march=cpu0 -mcpu=cpu032II -relocation-model=static -filetype=asm ch_call_compilerrt_func.ll -o -
	...
	ld	$4, 20($fp)
	ld	$5, 16($fp)
	jsub	__addsf3


For some brar-metal or embedded application, the C code doesn't need the
file and high-level IO in libc.
Libm provides a lots of functions to support software floating point beyond
basic operations [#math]_ .
Libc provides file, high-level IO functions and basic float functions [#clib]_ .

Cpu0 hires Compiler-rt/lib/builtins and the tiny single module printf-stdarg.c 
[#printf-stdarg]_ at this point.
Directory exlib/libsoftfloat/compiler-rt-12.x/builtins is a symbolic link to 
llvm-project/compiler-rt/lib/builtins which is the floating point library from
compiler-rt [#compiler-rt]_. The compiler-rt/lib/builtins is a 
target-independent C form of software float library implementation. Cpu0 
implements compiler-rt-12.x/cpu0/fp_mode.h only at this point for supporting 
this feature.

The code modified as follows,

.. rubric:: lbt/exlbt/libsoftfloat/compiler-rt-12.x/cpu0/fp_mode.c
.. literalinclude:: ../exlbt/libsoftfloat/compiler-rt-12.x/cpu0/fp_mode.c

.. rubric:: lbt/exlbt/libsoftfloat/compiler-rt-12.x/cpu0/abort.c
.. literalinclude:: ../exlbt/libsoftfloat/compiler-rt-12.x/cpu0/abort.c

.. rubric:: lbt/exlbt/libsoftfloat/compiler-rt-12.x/Makefile
.. literalinclude:: ../exlbt/libsoftfloat/compiler-rt-12.x/Makefile



Software Float Point Support
-----------------------------

.. rubric:: exlbt/include/math.h
.. literalinclude:: ../exlbt/include/math.h

.. rubric:: exlbt/include/stdio.h
.. literalinclude:: ../exlbt/include/stdio.h

.. rubric:: exlbt/include/stdlib.h
.. literalinclude:: ../exlbt/include/stdlib.h

.. rubric:: exlbt/include/string.h
.. literalinclude:: ../exlbt/include/string.h

.. rubric:: exlbt/libsoftfloat/compiler-rt/cpu0/abort.c
.. literalinclude:: ../exlbt/libsoftfloat/compiler-rt/cpu0/abort.c

.. rubric:: exlbt/libsoftfloat/compiler-rt/cpu0/fp_mode.c
.. literalinclude:: ../exlbt/libsoftfloat/compiler-rt/cpu0/fp_mode.c

.. rubric:: exlbt/input/ch_float_necessary.cpp
.. literalinclude:: ../exlbt/input/ch_float_necessary.cpp
    :start-after: /// start

.. rubric:: exlbt/input/Makefile.float-necessary
.. literalinclude:: ../exlbt/input/Makefile.float-necessary

Run as follows,

.. code-block:: console

  chungshu@ChungShudeMacBook-Air input % bash make.sh cpu032II be Makefile.float-necessary
  ...
  endian =  BigEndian
  ISR address:00020614
  0   /* 0: big endian, 1: little endian */

  chungshu@ChungShudeMacBook-Air verilog % iverilog -o cpu0IIs cpu0IIs.v 
  chungshu@ChungShudeMacBook-Air verilog % ./cpu0IIs
  WARNING: cpu0.v:489: $readmemh(cpu0.hex): Not enough words in the file for the requested range [0:524287].
  taskInterrupt(001)
  test_longlong_shift1() = 289
  test_longlong_shift2() = 22
  (int)test_shift_left<long long>(0x12, 4) = 288
  (int)test_shift_right<long long>(0x001666660000000a, 48) = 22
  (int)test_shift_right<unsigned long long>(0x001666660000000a, 48) = 22
  (int)test_add<float, float, float>(-2.2, 3.3) = 1
  (int)test_mul<float, float, float>(-2.2, 3.3) = -7
  (int)test_div<float, float, float>(-1.8, 0.5) = -3
  (int)test_add<double, double, float>(-2.2, 3.3) = 1
  (int)test_add<double, float, double>(-2.2, 3.3) = 1
  (int)test_add<float, float, double>(-2.2, 3.3) = 1
  (int)test_mul<double, float, double>(-2.2, 3.3) = -7
  (int)test_mul<float, float, double>(-2.2, 3.3) = -7
  (int)test_div<double, double, double>(-1.8, 0.5) = -3
  total cpu cycles = 240170              
  RET to PC < 0, finished!


.. rubric:: exlbt/input/ch_builtins.cpp
.. literalinclude:: ../exlbt/input/ch_builtins.cpp

.. rubric:: exlbt/input/Makefile.builtins
.. literalinclude:: ../exlbt/input/Makefile.builtins

Run as follows,

.. code-block:: console

  chungshu@ChungShudeMacBook-Air input % bash make.sh cpu032II be Makefile.builtins
  ...
  chungshu@ChungShudeMacBook-Air verilog % ./cpu0IIs
  WARNING: cpu0.v:489: $readmemh(cpu0.hex): Not enough words in the file for the requested range [0:524287].
  taskInterrupt(001)
  absvdi2_test(): PASS!
  absvsi2_test(): PASS!
  absvti2_test(): SKIPPED!
  adddf3vfp_test(): SKIPPED!
  addsf3vfp_test(): SKIPPED!
  addvdi3_test(): PASS!
  addvsi3_test(): PASS!
  addvti3_test(): SKIPPED!
  ashldi3_test(): PASS!
  ashlti3_test(): SKIPPED!
  ashrdi3_test(): PASS!
  ashrti3_test(): SKIPPED!
  clzdi2_test(): PASS!
  clzsi2_test(): PASS!
  clzti2_test(): SKIPPED!
  cmpdi2_test(): PASS!
  cmpti2_test(): SKIPPED!
  comparedf2_test(): PASS!
  comparesf2_test(): PASS!
  cpu_model_test(): SKIPPED!
  ctzdi2_test(): PASS!
  ctzsi2_test(): PASS!
  ctzti2_test(): SKIPPED!
  divdf3_test(): PASS!
  divdf3vfp_test(): SKIPPED!
  divdi3_test(): PASS!
  divmodsi4_test(): PASS!
  divmodti4_test(): SKIPPED!
  divsf3_test(): PASS!
  divsf3vfp_test(): SKIPPED!
  divsi3_test(): PASS!
  divtf3_test(): SKIPPED!
  divti3_test(): SKIPPED!
  total cpu cycles = 3412335             
  RET to PC < 0, finished!

.. [#gnu] https://en.wikipedia.org/wiki/GNU_Compiler_Collection#cite_note-55

.. [#toolchain] page 8 - 9 of  https://archive.fosdem.org/2018/schedule/event/crosscompile/attachments/slides/2107/export/events/attachments/crosscompile/slides/2107/How_to_cross_compile_with_LLVM_based_tools.pdf

.. [#libgcc] https://gcc.gnu.org/onlinedocs/gccint/Libgcc.html

.. [#soft-float-lib] https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html#Soft-float-library-routines

.. [#compiler-rt] http://compiler-rt.llvm.org/

.. [#math] https://www.programiz.com/c-programming/library-function/math.h

.. [#clib] https://www.cplusplus.com/reference/clibrary

.. [#printf-stdarg] https://github.com/atgreen/FreeRTOS/blob/master/Demo/CORTEX_STM32F103_Primer_GCC/printf-stdarg.c
