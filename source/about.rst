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

Version 3.6.0, Not release yet
  Porting to lld 3.6.

Licensing
---------

http://llvm.org/docs/DeveloperPolicy.html#license


Outline of Chapters
-------------------

.. _about-f1: 
.. figure:: ../Fig/about/1.png
  :scale: 100 %
  :align: center

  Code generation and execution flow

The upper half of :num:`Figure #about-f1` is the work flow and software package 
of a computer program be generated and executed. IR stands for Intermediate 
Representation. 
The lower half is this book's work flow and software package of the toolchain 
extended implementation based on llvm. Except clang, the other blocks need to 
be extended for a new backend development. The hex is the ascii file format 
using '0' to '9' and 'a' to 'f' for hexadecimal value representation since 
the verilog language machine uses it as input file.
The Cpu0 llvm backend can be find on 
http://jonathan2251.github.io/lbd/index.html.

This book include:

1. ELF linker for Cpu0 which extended from lld. Chapter 2.
2. The elf2hex extended from llvm-objump. Chapter 2.
3. Optimization. Chapter 3.
4. Porting C standard library from avr libc and software floating point library
   from LLVM compiler-rt.

With these implementation, reader can generate Cpu0 machine code through Cpu0 
llvm backend compiler, linker and elf2hex, then see how it runs on your 
computer. 
The pdf and epub are also available in the web.

Outline of Chapters
-------------------

:ref:`sec-lld`:

Develop ELF linker for Cpu0 backend based on lld project.  

:ref:`sec-opt`:

Backend independent optimaization. Under working and I need to avoid something I
know since I working with a proprietary company.

:ref:`sec-lib`:

Software floating point library and standard C library supporting. Under working.

