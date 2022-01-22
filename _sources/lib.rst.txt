.. _sec-lib:

Library
========

.. contents::
   :local:
   :depth: 4

Since Cpu0 has not hardware float point instructions, it needs soft float point
library to finish the floating point operation. LLVM compiler-rt project include
software floating point library implementation :numref:`lib-empty` , so we choose it as the 
implementation. 

Since compiler-rt uses unix/linux rootfs structure, we fill the gap by add few
empty include-files in exlbt/include.

.. _lib-empty:
.. figure:: ../Fig/empty.png

  compiler-rt\/lib\/builtins' software float library
  
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
implements compiler-rt-12.x/cpu0/abort.c only at this point for supporting 
this feature.


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

.. rubric:: lbt/exlbt/libsoftfloat/compiler-rt-12.x/Makefile
.. literalinclude:: ../exlbt/libsoftfloat/compiler-rt-12.x/Makefile

.. rubric:: exlbt/input/ch_float.cpp
.. literalinclude:: ../exlbt/input/ch_float.cpp
    :start-after: /// start

.. rubric:: exlbt/input/Makefile.float
.. literalinclude:: ../exlbt/input/Makefile.float

Run as follows,

.. code-block:: console

  chungshu@ChungShudeMacBook-Air input % bash make.sh cpu032II be Makefile.float
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


The following sanitizer_printf.cpp extended from compiler-rt can support 
printf("%lld"). It's implementation calling some floating lib functions
in compiler-rt/lib/builtins.

.. rubric:: exlbt/input/sanitizer_internal_defs.h
.. literalinclude:: ../exlbt/input/sanitizer_internal_defs.h

.. rubric:: exlbt/input/sanitizer_printf.cpp
.. literalinclude:: ../exlbt/input/sanitizer_printf.cpp

.. rubric:: exlbt/input/ch_hello.c
.. literalinclude:: ../exlbt/input/ch_hello.c

.. rubric:: exlbt/input/Makefile.sanitizer-printf
.. literalinclude:: ../exlbt/input/Makefile.sanitizer-printf


.. code-block:: console

  cschen@cschendeiMac input %  bash make.sh cpu032I le Makefile.slinker

  cschen@cschendeiMac verilog % ./cpu0Is
  ...
  Hello world!
  a: 100000007FFFFFFF, 100000007fffffff, 1152921506754330623
  b: 10000000, 268435456
  total cpu cycles = 1266990             
  RET to PC < 0, finished!

The exlbt/input/compiler-rt-test/builtins/Unit copied from 
compiler-rt/test/builtins/Unit as follows,

.. rubric:: exlbt/input/ch_builtins.cpp
.. literalinclude:: ../exlbt/input/ch_builtins.cpp

.. rubric:: exlbt/input/Makefile.builtins
.. literalinclude:: ../exlbt/input/Makefile.builtins

Run as follows,

.. code-block:: console

  chungshu@ChungShudeMacBook-Air input % bash make.sh cpu032II be Makefile.builtins
  ...
  chungshu@ChungShudeMacBook-Air verilog % ./cpu0IIs
  ...
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
  bswapdi2_test(): PASS!
  bswapsi2_test(): PASS!
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
  eqdf2vfp_test(): SKIPPED!
  eqsf2vfp_test(): SKIPPED!
  eqtf2_test(): SKIPPED!
  extenddftf2_test(): SKIPPED!
  extendhfsf2_test(): PASS!
  extendhftf2_test(): SKIPPED!
  extendsfdf2vfp_test(): SKIPPED!
  extendsftf2_test(): SKIPPED!
  gedf2vfp_test(): SKIPPED!
  gesf2vfp_test(): SKIPPED!
  getf2_test(): SKIPPED!
  gtdf2vfp_test(): SKIPPED!
  gtsf2vfp_test(): SKIPPED!
  gttf2_test(): SKIPPED!
  ledf2vfp_test(): SKIPPED!
  lesf2vfp_test(): SKIPPED!
  letf2_test(): SKIPPED!
  lshrdi3_test(): PASS!
  lshrti3_test(): SKIPPED!
  ltdf2vfp_test(): SKIPPED!
  ltsf2vfp_test(): SKIPPED!
  lttf2_test(): SKIPPED!
  moddi3_test(): PASS!
  modsi3_test(): PASS!
  modti3_test(): SKIPPED!
  muldf3vfp_test(): SKIPPED!
  muldi3_test(): PASS!
  mulodi4_test(): PASS!
  mulosi4_test(): PASS!
  muloti4_test(): SKIPPED!
  mulsf3vfp_test(): SKIPPED!
  multf3_test(): SKIPPED!
  multi3_test(): SKIPPED!
  mulvdi3_test(): PASS!
  mulvsi3_test(): PASS!
  mulvti3_test(): SKIPPED!
  nedf2vfp_test(): SKIPPED!
  negdf2vfp_test(): SKIPPED!
  negdi2_test(): PASS!
  negsf2vfp_test(): SKIPPED!
  negti2_test(): SKIPPED!
  negvdi2_test(): PASS!
  negvsi2_test(): PASS!
  negvti2_test(): SKIPPED!
  nesf2vfp_test(): SKIPPED!
  netf2_test(): SKIPPED!
  subdf3vfp_test(): SKIPPED!
  subsf3vfp_test(): SKIPPED!
  subtf3_test(): SKIPPED!
  subvdi3_test(): PASS!
  subvsi3_test(): PASS!
  subvti3_test(): SKIPPED!
  trampoline_setup_test(): SKIPPED!
  truncdfhf2_test(): PASS!
  truncdfsf2_test(): PASS!
  truncdfsf2vfp_test(): SKIPPED!
  truncsfhf2_test(): PASS!
  trunctfdf2_test(): SKIPPED!
  trunctfhf2_test(): SKIPPED!
  trunctfsf2_test(): SKIPPED!
  ucmpdi2_test(): PASS!
  ucmpti2_test(): SKIPPED!
  udivdi3_test(): PASS!
  udivmodsi4_test(): PASS!
  udivmodti4_test(): SKIPPED!
  udivsi3_test(): PASS!
  udivti3_test(): SKIPPED!
  umoddi3_test(): PASS!
  umodsi3_test(): PASS!
  umodti3_test(): SKIPPED!
  unorddf2vfp_test(): SKIPPED!
  unordsf2vfp_test(): SKIPPED!
  unordtf2_test(): SKIPPED!
  ...          
  RET to PC < 0, finished!


.. [#gnu] https://en.wikipedia.org/wiki/GNU_Compiler_Collection#cite_note-55

.. [#toolchain] page 8 - 9 of  https://archive.fosdem.org/2018/schedule/event/crosscompile/attachments/slides/2107/export/events/attachments/crosscompile/slides/2107/How_to_cross_compile_with_LLVM_based_tools.pdf

.. [#libgcc] https://gcc.gnu.org/onlinedocs/gccint/Libgcc.html

.. [#soft-float-lib] https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html#Soft-float-library-routines

.. [#compiler-rt] http://compiler-rt.llvm.org/

.. [#math] https://www.programiz.com/c-programming/library-function/math.h

.. [#clib] https://www.cplusplus.com/reference/clibrary

.. [#printf-stdarg] https://github.com/atgreen/FreeRTOS/blob/master/Demo/CORTEX_STM32F103_Primer_GCC/printf-stdarg.c
