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

Both the compiler-rt and avr libc porting is under going, it's not finished.
The flow as follows,

.. _lib-f-flow: 
.. figure:: ../Fig/lib/1.png
  :scale: 100 %
  :align: center

  libc/softfloat library flow

The llvm-link which introduced at last chapter can be hired for optimization.


Compiler-rt
-------------

Directory libex/libsoftfloat/compiler-rt include the floating point library
support for Cpu0 backend. The compiler-rt [#compiler-rt]_ version we use is 
llvm 3.5 release.


Avr libc
---------

Directory libex/libc/avr-libc-1.8.1 include the libc porting.

AVR Libc is a Free Software project whose goal is to provide a high quality C 
library for use with GCC on Atmel AVR microcontrollers. AVR Libc is licensed 
under a single unified license. This so-called modified Berkeley license is 
intented to be compatible with most Free Software licenses like the GPL, yet 
impose as little restrictions for the use of the library in closed-source 
commercial applications as possible [#avr-libc-license]_.

The source code can be download from here [#avr-libc-download]_. 
Document are here [#avr-libc-doc-web]_ [#avr-libc-doc-pdf]_.


Software Float Point Support
-----------------------------

.. rubric:: exlbt/input/ch_float.cpp
.. literalinclude:: ../exlbt/input/ch_float.cpp
    :start-after: /// start

.. rubric:: exlbt/input/build-float.sh
.. literalinclude:: ../exlbt/input/build-float.sh

Run as follows,

.. code-block:: console

  JonathantekiiMac:input Jonathan$ bash build-float.sh cpu032II be
  ...
  endian =  BigEndian
  0   /* 0: big endian, 1: little endian */

  JonathantekiiMac:input Jonathan$ iverilog -o cpu0IIs cpu0IIs.v 
  JonathantekiiMac:input Jonathan$ ./cpu0IIs
  114-43-184-210:verilog Jonathan$ ./cpu0IIs
  WARNING: ./cpu0.v:458: $readmemh(cpu0.hex): Not enough words in the file for 
  the requested range [0:524287].
  taskInterrupt(001)
  test_longlong_shift1() = 289
  test_longlong_shift2() = 22
  test_float_to_int() = -2
  test_double_to_int() = -4
  test_double_to_int_2() = 3
  test_float_add() = 1
  test_float_mul() = 31
  test_float_div() = -6
  test_double_add() = 5
  test_double_mul() = -31
  test_double_div() = -6
  total cpu cycles = 104105            
  RET to PC < 0, finished!



.. [#compiler-rt] http://compiler-rt.llvm.org/

.. [#avr-libc-license] http://www.nongnu.org/avr-libc/

.. [#avr-libc-download] http://download.savannah.gnu.org/releases/avr-libc/

.. [#avr-libc-doc-web] http://www.atmel.com/webdoc/AVRLibcReferenceManual/index.html

.. [#avr-libc-doc-pdf] http://courses.cs.washington.edu/courses/csep567/04sp/pdfs/avr-libc-user-manual.pdf


