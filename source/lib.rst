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

.. rubric:: exlbt/input/ch_float_necessary.cpp
.. literalinclude:: ../exlbt/input/ch_float_necessary.cpp
    :start-after: /// start

.. rubric:: exlbt/input/build-float-necessary.sh
.. literalinclude:: ../exlbt/input/build-float-necessary.sh

Run as follows,

.. code-block:: console

  JonathantekiiMac:input Jonathan$ bash build-float-necessary.sh cpu032II be
  ...
  endian =  BigEndian
  0   /* 0: big endian, 1: little endian */

  JonathantekiiMac:input Jonathan$ iverilog -o cpu0IIs cpu0IIs.v 
  JonathantekiiMac:input Jonathan$ ./cpu0IIs
  114-43-184-210:verilog Jonathan$ ./cpu0IIs
  ARNING: cpu0.v:487: $readmemh(cpu0.hex): Not enough words in the file for the requested range [0:524287].
  taskInterrupt(001)
  (int)test_shift_left<long long>(0x12, 4) = 288
  (int)test_shift_right<long long>(0x001666660000000a, 48) = 22
  (int)test_shift_right<unsigned long long>(0x001666660000000a, 48) = 22
  (int)test_add<float, float, float>(-2.2, 3.3) = 1
  (int)test_mul<float, float, float>(-2.2, 3.3) = -7
  (int)test_div<float, float, float>(-1.8, 0.5) = -3
  (int)test_add<double, double, float>(-2.2, 3.3) = 1
  (int)test_mul<float, float, double>(-2.2, 3.3) = -7
  (int)test_mul<float, float, double>(-2.2, 3.3) = 0
  (int)test_div<double, double, double>(-1.8, 0.5) = -3
  total cpu cycles = 182585
  RET to PC < 0, finished!



.. [#compiler-rt] http://compiler-rt.llvm.org/

.. [#avr-libc-license] http://www.nongnu.org/avr-libc/

.. [#avr-libc-download] http://download.savannah.gnu.org/releases/avr-libc/

.. [#avr-libc-doc-web] http://www.atmel.com/webdoc/AVRLibcReferenceManual/index.html

.. [#avr-libc-doc-pdf] http://courses.cs.washington.edu/courses/csep567/04sp/pdfs/avr-libc-user-manual.pdf


