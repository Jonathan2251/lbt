.. _sec-about:

About
======

.. contents::
   :local:
   :depth: 4

Authors
-------

.. image:: ../Fig/Author_ChineseName.png
   :align: right

Chen Chung-Shu

gamma_chen@yahoo.com.tw

http://jonathan2251.github.io/web/index.html


Acknowledgments
---------------

I would like to thank Sean Silva, chisophugis@gmail.com, for his help, 
encouragement, and assistance with the Sphinx document generator.  
Without his help, this book would not have been finished and published online. 
Also thanking those corrections from readers who make the book more accurate.


Build steps
-----------

https://github.com/Jonathan2251/lbt/blob/master/README.md


Revision history
----------------

Version 12.0.5, not released yet.

Version 12.0.4, Released January 22, 2022.

  sanitizer-printf for supporting printf("%lld") or "%llX", ..., etc.
  Pass test cases in compiler-rt-test/builtins/Unit include type float and double
  exclude complex.

Version 12.0.3, Released January 9, 2022.

  Expand memory size of cpu0.v to 0x1000000, 24-bit.
  Add all compiler-rt-test/builtins/Unit/\*.c.
  
Version 12.0.2, Release December 18, 2021.
  
  Replace bash with Makefile.
  Add builtins-cpu0.c for clang regression test.

Version 12.0.1, Release December 12, 2021.

  Add target Cpu0 to clang

Version 12.0.0, Release Auguest 11, 2021.

Version 3.9.1, Released April 29, 2020

  Enable tailcall test option in build-slinker.sh

Version 3.9.0, Released November 22, 2016

  Porting to llvm 3.9.

Version 3.7.4, Released September 22, 2016

  Split elf2hex-dlinker.cpp from elf2hex.cpp in exlbt/elf2hex.

Version 3.7.3, Released July 20, 2016

  Refine code-block according sphinx lexers.
  Add search this book.
  
Version 3.7.2, Released June 29, 2016

  Dynamic linker change display from ret \$t9 to jr \$t9.
  Move llvm-objdump -elf2hex to elf2hex.
  Upgrade sphinx to 1.4.4.

Version 3.7.1, Released November 7, 2015

  Remove EM_CPU0_EL.
  Add IR blockaddress and indirectbr support.
  Add ch_9_3_detect_exception.cpp test.
  Change display "ret $rx" to "jr $rx" where $rx is not $lr.
  Add Phi node test.

Version 3.7.0, Released September 24, 2015

  Porting to lld 3.7.

Version 3.6.2, Released May 4, 2015

  Move some test from lbt to lbd.
  Remove warning in build Cpu0 code.

Version 3.6.1, Released March 22, 2015
  Correct typing.

Version 3.6.0, Released March 8, 2015
  Porting to lld 3.6.

Licensing
---------

http://llvm.org/docs/DeveloperPolicy.html#license


Outline of Chapters
-------------------

.. _about-f1: 
.. figure:: ../Fig/about/mywork_1.png
  :scale: 100 %
  :align: center

  Code generation and execution flow

The upper half of :numref:`about-f1` is the work flow and software package 
of a computer program be generated and executed. IR stands for Intermediate 
Representation. 
The lower half is this book's work flow and software package of the toolchain 
extended implementation based on llvm. Except clang, the other blocks need to 
be extended for a new backend development. This book implement the green boxes
part.
The Cpu0 llvm backend can be find on 
http://jonathan2251.github.io/lbd/index.html.

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

The libgcc's Integer plus Soft float library [#libgcc]_ [#integer-lib]_ 
[#soft-float-lib]_ are equal to functions of compiler-rt's builtins.

This book include:

1. Add Cpu0 target to clang.
2. The elf2hex extended from llvm-objump. Chapter 3.
3. Optimization. Chapter 4.
4. Porting C standard library from avr libc and software floating point library
   from LLVM compiler-rt.

With these implementation, reader can generate Cpu0 machine code through Cpu0 
llvm backend compiler, linker and elf2hex, then see how it runs on your 
computer. 

:ref:`sec-clang`:

Add Cpu0 target to clang.

:ref:`sec-lld`:

Develop ELF linker for Cpu0 backend based on lld project.  

:ref:`sec-opt`:

Backend independent optimaization.

:ref:`sec-lib`:

Software floating point library and standard C library supporting. Under working.



.. [#gnu] https://en.wikipedia.org/wiki/GNU_Compiler_Collection#cite_note-55

.. [#toolchain] page 8 - 9 of  https://archive.fosdem.org/2018/schedule/event/crosscompile/attachments/slides/2107/export/events/attachments/crosscompile/slides/2107/How_to_cross_compile_with_LLVM_based_tools.pdf

.. [#libgcc] https://gcc.gnu.org/onlinedocs/gccint/Libgcc.html

.. [#integer-lib] https://gcc.gnu.org/onlinedocs/gccint/Integer-library-routines.html#Integer-library-routines

.. [#soft-float-lib] https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html#Soft-float-library-routines
