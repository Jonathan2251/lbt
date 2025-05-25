.. _sec-lld:

Cpu0 ELF linker
===============

.. contents::
   :local:
   :depth: 4

LLD changes frequently, and the figures in this chapter are not up to date. Like
LLVM, the LLD linker includes support for multiple targets in ELF format.

The term "Cpu0 backend" used in this chapter can refer to ELF format handling for
the Cpu0 target under LLD, the LLVM compiler backend, or both. It is assumed that
readers will understand the intended meaning from the context.

.. _lld-f-workflow: 
.. figure:: ../Fig/about/mywork_1.png
  :scale: 100 %
  :align: center

  Code generation and execution flow

As depicted in :numref:`lld-f-workflow` of chapter About, beside LLVM backend, we
implement ELF linker and elf2hex to run on Cpu0 Verilog simulator.

This chapter extends LLD to support the Cpu0 backend as well as elf2hex to replace
the Cpu0 loader.

After linking with LLD, the program with global variables can be allocated in ELF
file format layout. This means relocation records of global variables are resolved.

In addition, elf2hex is implemented to generate Hex files from ELF.

With these two tools, global variables in sections .data and .rodata can be accessed
and transferred to Hex files which feed the Verilog Cpu0 machine running on your
PC/Laptop.

As previous chapters mentioned, Cpu0 has two relocation models for static and
dynamic linking respectively, controlled by the option ``-relocation-model`` in
``llc``. This chapter supports static linking.

For more about LLD, please refer to the LLD website here [#lldweb]_ and the LLD
install requirements on Linux here [#lld-install]_.

Currently, LLD can be built by gcc and clang on Ubuntu. On iMac, LLD can be built
by clang with the Xcode version as described in the next subsection.

If you run inside a Virtual Machine (VM), please set the physical memory size over
1GB to avoid insufficient memory link errors.

ELF to Hex
-----------

The code to convert ELF file format into Hex format for Cpu0 as follows,

.. rubric:: exlbt/elf2hex/CMakeLists.txt
.. literalinclude:: ../exlbt/elf2hex/CMakeLists.txt

.. rubric:: exlbt/elf2hex/elf2hex.h
.. literalinclude:: ../exlbt/elf2hex/elf2hex.h

.. rubric:: exlbt/elf2hex/elf2hex.cpp
.. literalinclude:: ../exlbt/elf2hex/elf2hex.cpp

To support the commands **llvm-objdump -d** and **llvm-objdump -t** for Cpu0, 
the following code is added to llvm-objdump.cpp:

.. rubric:: exlbt/llvm-objdump/llvm-objdump.cpp
.. code-block:: c++

  case ELF::EM_CPU0: //Cpu0


Create Cpu0 backend under LLD
-----------------------------

LLD introduction
~~~~~~~~~~~~~~~~

In general, the linker performs relocation record resolution as described in 
the ELF support chapter. Some optimizations cannot be completed during the 
compiler stage. One such optimization opportunity in the linker is Dead Code 
Stripping, which is explained as follows:

.. rubric:: Dead code stripping - example (modified from llvm lto document web)

.. rubric:: a.h
.. literalinclude:: ../exlbt/input/a.h

.. rubric:: a.cpp
.. literalinclude:: ../exlbt/input/a.cpp

.. rubric:: ch13_1.cpp
.. literalinclude:: ../exlbt/input/ch13_1.cpp

The above code can be simplified to :numref:`lld-deadcodestripping` to perform
mark and sweep in the graph for Dead Code Stripping.

.. _lld-deadcodestripping: 
.. figure:: ../Fig/lld/deadcodestripping.png
  :scale: 40 %
  :align: center

  Atom classified (from lld web)


As shown in the above example, the function `foo2()` is an isolated node without
any references. It is dead code and should be removed during linker optimization.
We tested this example using `Makefile.ch13_1` and found that `foo2()` was not
removed.

There are two possible reasons for this behavior. First, the dead code stripping
optimization in LLD might not be enabled by default in the command line. Second,
LLD may not have implemented this optimization yet, which is reasonable given
that LLD was in its early stages of development at the time.

We did not investigate further since the Cpu0 backend tutorial only requires a
linker to complete relocation record resolution and to run the program on a PC.

Note that `llvm-linker` works at the LLVM IR level and performs linker
optimizations there. However, if you only have object files (e.g., `a.o`), the
native linker (such as LLD) has the opportunity to perform dead code stripping,
while the IR linker cannot.

Static linker
~~~~~~~~~~~~~

Let's run the static linker first and explain it next.

File ``printf-stdarg.c`` [#printf-stdarg]_ comes from an internet download and
is under the GPL2 license. GPL2 is more restrictive than the LLVM license.

.. rubric:: exlbt/input/printf-stdarg-1.c
.. literalinclude:: ../exlbt/input/printf-stdarg-1.c
    :start-after: /// start

.. rubric:: exlbt/input/printf-stdarg-def.c
.. literalinclude:: ../exlbt/input/printf-stdarg-def.c
    :start-after: /// start

.. rubric:: exlbt/input/printf-stdarg.c
.. literalinclude:: ../exlbt/input/printf-stdarg.c
    :start-after: /// start

.. rubric:: exlbt/input/start.cpp
.. literalinclude:: ../exlbt/input/start.cpp
    :start-after: /// start

.. rubric:: exlbt/input/lib_cpu0.c
.. literalinclude:: ../exlbt/input/lib_cpu0.c

.. rubric:: exlbt/input/Common.mk
.. literalinclude:: ../exlbt/input/Common.mk

With the ``printf()`` function from the GPL source code, we can write more test
code to verify the previously generated LLVM Cpu0 backend program. The
following code serves this purpose.

.. rubric:: exlbt/input/debug.cpp
.. literalinclude:: ../exlbt/input/debug.cpp
    :start-after: /// start
    
.. rubric:: exlbt/input/ch_lld_staticlink.h
.. literalinclude:: ../exlbt/input/ch_lld_staticlink.h
    :start-after: /// start

.. rubric:: exlbt/input/ch_lld_staticlink.cpp
.. literalinclude:: ../exlbt/input/ch_lld_staticlink.cpp
    :start-after: /// start

.. rubric:: exlbt/input/ch_slinker.cpp
.. literalinclude:: ../exlbt/input/ch_slinker.cpp
    :start-after: /// start

.. rubric:: exlbt/input/Makefile.slinker
.. literalinclude:: ../exlbt/input/Makefile.slinker

.. rubric:: exlbt/input/make.sh
.. literalinclude:: ../exlbt/input/make.sh
  
.. code-block:: console

  $ cd ~/git/lbd/lbdex/verilog
  $ make
  $ cd  ~/git/lbt/exlbt/input
  $ pwd
  $HOME/git/lbt/exlbt/input
  $ bash make.sh cpu032I el Makefile.slinker
  ...
  endian =  LittleEndian
  ISR address:00020780
  1   /* 0: big endian, 1: little endian */
  $ cd ~/git/lbd/lbdex/verilog
  $ pwd
  $HOME/git/lbd/lbdex/verilog
  $ ./cpu0Is 
  WARNING: cpu0.v:487: $readmemh(cpu0.hex): Not enough words in the file for the requested range [0:524287].
  taskInterrupt(001)
  74
  7
  0
  0
  253
  3
  1
  13
  3
  -126
  130
  -32766
  32770
  393307
  16777222
  -3
  -4
  51
  2
  3
  1
  2147483647
  -2147483648
  9
  12
  5
  0
  31
  49
  test_nolld(): PASS
  __CPU0EL__
  Hello world!
  printf test
  (null) is null pointer
  5 = 5
  -2147483647 = - max int
  char a = 'a'
  hex ff = ff
  hex 00 = 00
  signed -3 = unsigned 4294967293 = hex fffffffd
  0 message(s)
  0 message(s) with %
  justif: "left      "
  justif: "     right"
   3: 0003 zero padded
   3: 3    left justif.
   3:    3 right justif.
  -3: -003 zero padded
  -3: -3   left justif.
  -3:   -3 right justif.
  global variable gI = 100, PASS
  test_ctrl2(): a = 1, b = 0, c = 1, d = 0, PASS
  test_phinode(3, 1) = 3, PASS
  test_blockaddress(1) = 1, PASS
  test_blockaddress(2) = 2, PASS
  test_longbranch() = 0, PASS
  test_func_arg_struct() = 0, PASS
  test_constructor() = 0, PASS
  test_template() = 15, PASS
  test_template_ll() = 0xFFFFFFFF-FFFFFFFF, PASS
  test_tailcall(5) = 120, PASS
  exceptionOccur= 1, PASS
  exceptionOccur= 0, PASS
  inlineasm_global() = 4, PASS
  20
  10
  5
  test_cpp_polymorphism() = 0, PASS
  taskInterrupt(011)
  Software interrupt
  taskInterrupt(011)
  Harware interrupt 0
  taskInterrupt(011)
  Harware interrupt 1
  ...             
  RET to PC < 0, finished!

The above test includes verification of the ``printf`` format.
Let’s check the result by comparing it with the output of the PC program
``printf-stdarg-1.c`` as follows,

.. code-block:: console

  $ cd  ~/git/lbt/exlbt/input
  $ pwd
  $HOME/git/lbt/exlbt/input
  $ clang printf-stdarg-1.c
  printf-stdarg-1.c:58:19: warning: incomplete format specifier [-Wformat]
    printf("%d %s(s)%", 0, "message");
                    ^
  1 warning generated.
  $ ./a.out
  Hello world!
  printf test
  (null) is null pointer
  5 = 5
  -2147483647 = - max int
  char a = 'a'
  hex ff = ff
  hex 00 = 00
  signed -3 = unsigned 4294967293 = hex fffffffd
  0 message(s)
  0 message(s) with \%
  justif: "left      "
  justif: "     right"
   3: 0003 zero padded
   3: 3    left justif.
   3:    3 right justif.
  -3: -003 zero padded
  -3: -3   left justif.
  -3:   -3 right justif.

As described above, by leveraging open source code, Cpu0 gained a more stable
implementation of the ``printf()`` function. Once the Cpu0 backend can translate
the open source C ``printf()`` program into machine instructions, the LLVM
Cpu0 backend can be effectively verified using ``printf()``.

With the high-quality open source ``printf()`` code, the Cpu0 toolchain extends
from just a compiler backend to also support the C standard library.
(Note that while some GPL open source code may be of lower quality, many are
well-written.)

The message "Overflow exception is printed twice" means the ISR() in
``debug.cpp`` is called twice from ``ch4_1_2.cpp``.

The printed messages ``taskInterrupt(001)`` and ``taskInterrupt(011)`` are just
trace messages from ``cpu0.v`` code.

Dynamic linker
~~~~~~~~~~~~~~

The dynamic linker demonstration was removed from version 3.9.0 because its
implementation with lld 3.9 was unclear and required extensive additions to
``elf2hex``, Verilog, and the Cpu0 backend in lld.

However, it can still be run with LLVM 3.7 using the following command.

.. code-block:: console

  1-160-136-173:test Jonathan$ pwd
  /Users/Jonathan/test
  1-160-136-173:test Jonathan$ git clone https://github.com/Jonathan2251/lbd
  1-160-136-173:test Jonathan$ git clone https://github.com/Jonathan2251/lbt
  1-160-136-173:test Jonathan$ cd lbd
  1-160-136-173:lbd Jonathan$ pwd
  /Users/Jonathan/test/lbd
  1-160-136-173:lbd Jonathan$ git checkout release_374
  1-160-136-173:lbd Jonathan$ cd ../lbt
  1-160-136-173:test Jonathan$ git checkout release_374
  1-160-136-173:lbt Jonathan$ make html

Then read the corresponding section in ``lld.html`` for more details.

Summary
--------

Create a new backend based on LLVM
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Thanks to the LLVM open source project, writing a linker and ELF-to-Hex tools
for a new CPU architecture is both easy and reliable.

Combined with the LLVM Cpu0 backend code and Verilog code programmed in previous
chapters, we designed a software toolchain to compile C/C++ code, link it, and
run it on the Verilog Cpu0 simulator without any real hardware investment.

If you buy FPGA development hardware, we believe this code can run on an FPGA
CPU even though we did not test it ourselves.

Extending system programming toolchains to support a new CPU instruction set
can be completed just like we have shown here.

School knowledge of system programming, compiler, linker, loader, computer
architecture, and CPU design has been translated into real work and demonstrated
running in practice. Now this knowledge is not limited to paper.

We designed it, programmed it, and ran it in the real world.

The total code size of the LLVM Cpu0 backend compiler, Cpu0 LLD linker, elf2hex,
and Cpu0 Verilog code is around 10,000 lines including comments.

In comparison, the total code size of Clang, LLVM, and LLD exceeds 1,000,000
lines excluding tests and documentation. 

This means the Cpu0 backend, based on Clang, LLVM, and LLD, is only about 1%
of the size of the entire infrastructure.

Moreover, the LLVM Cpu0 backend and LLD Cpu0 backend share about 70% similarity
with LLVM MIPS.

Based on this fact, we believe LLVM has a well-defined architecture for compiler
design.


Contribute Back to Open Source Through Working and Learning
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Finally, 10,000 lines of source code in the Cpu0 backend is very small for a UI
program, but it is quite complex for system programming based on LLVM.  
Open source code gives  
programmers the best opportunity to understand, enhance, and extend the code.  
However, documentation is the next most important factor to improve open source  
development.

The Open Source Organization recognized this and started the Open Source  
Document Project [#BSDLicense]_ [#docproj]_ [#freebsdLicense]_  
[#gnuLicense]_ [#fdl]_. Open Source has grown into a giant software infrastructure  
powered by companies [#apple-opensource]_ [#ibm-opensource]_, academic research,  
and countless passionate engineers.

It has ended the era when everyone tried to reinvent the wheel.  
Extending your software from reusable source code is the right way. Of course,  
if you work in business, you should consider open source licensing.

Anyone can contribute back to open source through the learning process.  
This book was written by learning the LLVM backend and contributing back to  
the LLVM open source project.

We believe this book cannot exist in traditional paper form, since few readers  
are interested in studying LLVM backend, despite many compiler books published.  
Therefore, it is published electronically and tries to follow the Open Document  
License Exception [#gnu-phi]_.

There is a gap between concept and realistic program implementation.  
For learning a large, complex software such as the LLVM backend the concept of 
compiler knowledge alone is not enough.

We all learned knowledge through books in and after school. If you cannot find  
a good way to produce documentation, consider writing documents like this book.  
This book’s documentation uses the Sphinx tool, just like the LLVM development  
team. Sphinx uses the restructured text format here [#rst-ref]_ [#rst-directives]_  
[#rst]_.

Appendix A of the lbd book explains how to install the Sphinx tool.

Documentation work helps you re-examine your software and improve your program  
structure, reliability, and—most importantly—extend your code to places you  
did not expect.

.. [#lldweb] http://lld.llvm.org/

.. [#lld-install] http://lld.llvm.org/getting_started.html#on-unix-like-systems

.. [#llvm-download] http://llvm.org/releases/download.html#3.5

.. [#effectiveC++] http://www.cplusplus.com/reference/memory/unique_ptr/

.. [#get] http://www.cplusplus.com/reference/memory/unique_ptr/get/

.. [#move] http://www.cplusplus.com/reference/utility/move/

.. [#pic-call-stub] Page 5-12 of http://www.linux-mips.org/pub/linux/mips/doc/ABI/mipsabi.pdf

.. [#printf-stdarg] https://github.com/atgreen/FreeRTOS/blob/master/Demo/CORTEX_STM32F103_Primer_GCC/printf-stdarg.c

.. [#BSDLicense] http://en.wikipedia.org/wiki/BSD_Documentation_License

.. [#docproj] http://www.freebsd.org/docproj/

.. [#freebsdLicense] http://www.freebsd.org/copyright/freebsd-doc-license.html

.. [#gnuLicense] http://en.wikipedia.org/wiki/GNU_Free_Documentation_License

.. [#fdl] http://www.gnu.org/copyleft/fdl.html

.. [#apple-opensource] http://www.apple.com/opensource/

.. [#ibm-opensource] https://www.ibm.com/developerworks/opensource/

.. [#gnu-phi] http://www.gnu.org/philosophy/free-doc.en.html

.. [#rst-ref] http://docutils.sourceforge.net/docs/ref/rst/restructuredtext.html

.. [#rst-directives] http://docutils.sourceforge.net/docs/ref/rst/directives.html

.. [#rst] http://docutils.sourceforge.net/rst.html

