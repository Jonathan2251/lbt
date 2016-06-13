.. _sec-about:

About
======

.. contents::
   :local:
   :depth: 4

Authors
-------

.. figure:: ../Fig/Author_ChineseName.png
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


Revision history
----------------

Version 3.7.2, Not release yet
  Dynamic linker change display from ret \$t9 to jr \$t9.
  Move llvm-objdump -elf2hex to elf2hex.

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
.. figure:: ../Fig/about/1.png
  :scale: 50
  :align: center

  Code generation and execution flow

The upper half of :num:`Figure #about-f1` is the work flow and software package 
of a computer program be generated and executed. IR stands for Intermediate 
Representation. 
The lower half is this book's work flow and software package of the toolchain 
extended implementation based on llvm. Except clang, the other blocks need to 
be extended for a new backend development. This book implement the green boxes
part.
The Cpu0 llvm backend can be find on 
http://jonathan2251.github.io/lbd/index.html.

This book include:

1. The elf2hex extended from llvm-objump. Chapter 2.
2. Optimization. Chapter 3.
3. Porting C standard library from avr libc and software floating point library
   from LLVM compiler-rt.

With these implementation, reader can generate Cpu0 machine code through Cpu0 
llvm backend compiler, linker and elf2hex, then see how it runs on your 
computer. 

:ref:`sec-lld`:

Develop ELF linker for Cpu0 backend based on lld project.  

:ref:`sec-opt`:

Backend independent optimaization.

:ref:`sec-lib`:

Software floating point library and standard C library supporting. Under working.

