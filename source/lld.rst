.. _sec-lld:

Cpu0 ELF linker
===============

.. contents::
   :local:
   :depth: 4

LLD changes quickly and the figures of this chapter is not up to date.
Like llvm, lld linker include a couple of target in ELF format handling.
The term Cpu0 backend used in this chapter can refer to the ELF format handling 
for Cpu0 target machine under lld, llvm compiler backend, or both. But
supposing readers will easy knowing what it refer to.

.. _lld-f-workflow: 
.. figure:: ../Fig/about/1.png
  :scale: 100 %
  :align: center

  Code generation and execution flow

As depicted in :numref:`lld-f-workflow` of chapter About. Beside llvm backend,
we implement ELF linker and elf2hex to run on Cpu0 verilog simulator.
This chapter extends lld to support Cpu0 backend as well as elf2hex to replace
Cpu0 loader.
After link with lld,
the program with global variables can be allocated in ELF file format layout. 
Meaning the relocation records of global variables is resolved. In addition, 
elf2hex is implemented for supporting generate Hex file from ELF.
With these two tools supported, the global variables exists in 
section .data and .rodata can be accessed and transfered to Hex file which feeds  
to Verilog Cpu0 machine and run on your PC/Laptop.

As the previouse chapters mentioned, Cpu0 has two relocation models for static 
link and dynamic link, respectively, which controlled by option 
``-relocation-model`` in ``llc``. 
This chapter supports the static link fully, and part of dynamic link for 
demostration purpose. Since dynamic link needs the OS involement and the 
Cpu0 is run on Verilog bare metal simulator, the dynamic linker program 
is added and the Cpu0 Verilog code is extended to demostrate these Cpu0 PIC mode 
instructions are work correctly in dynamice link. 
However, these part of Cpu0 Verilog codes are not needed in a real machine with 
OS support, and the Cpu0 lld's and elf2hex's dynamic linker function is not full 
implemented. 
They are programmed by a specific shared library name since the shared library 
locating needs the OS's help (files management is part of OS's job). 
Without OS, these things cannot be solved and dynamic link is impossible to 
finish.
Anyway, for the dynamic link demostration, we can implement dynamic linker 
program and adapt lld, elf2hex, and Cpu0 Verilog code to support a specific 
shared library and verify the dynamic link result.
In reality, the Micro CPUs without OS or tiny OS inside only support static 
link for C language.

About lld please refer LLD web site here [#lldweb]_ and LLD install requirement 
on Linux here [#lld-install]_. 
Currently, lld can be built by: gcc and clang 3.5 compiler on Ubuntu, and gcc 
on Fedora, as I have tried. 
On iMac, lld can be built by clang with the Xcode version as the next sub 
section.
If you run with Virtual Machine (VM), please keep your phisical memory size 
setting over 1GB to avoid insufficient memory link error.

ELF to Hex
-----------

Copy exlbt/elf2hex to llvm/test/src/tools/ to supporting ELF to 
Hex for Cpu0 backend as follows,

.. code-block:: console

  1-160-136-173:tools Jonathan$ pwd
  /Users/Jonathan/llvm/test/src/tools
  1-160-136-173:tools Jonathan$ cp -rf ~/test/exlbt/elf2hex .

.. rubric:: exlbt/elf2hex/CMakeLists.txt
.. literalinclude:: ../exlbt/elf2hex/CMakeLists.txt

.. rubric:: exlbt/elf2hex/LLVMBuild.txt
.. literalinclude:: ../exlbt/elf2hex/LLVMBuild.txt

.. rubric:: exlbt/elf2hex/elf2hex.h
.. literalinclude:: ../exlbt/elf2hex/elf2hex.h

.. rubric:: exlbt/elf2hex/elf2hex.cpp
.. literalinclude:: ../exlbt/elf2hex/elf2hex.cpp

The code included in "#ifdef DLINK" are for dynamic linker support.
The elf2hex.h supports both endian dump.


Create Cpu0 backend under LLD
-----------------------------

Setup Cpu0 backend under lld
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

LLD project is underdevelopment and can be compiled only with c++11 standard 
(C++2011 year announced standard). 
For iMac, our software is OS X version 10.9.1 and Xcode version 5.0.2. 
For old iMac software version, you can install VM (such as Virtual Box) and 
build lld as Linux platform. Please download lld from llvm web 
[#llvm-download]_ and put lld 
souce code on {llvm-src}/tools/lld just like we download llvm and clang as 
shown in Appendex A of book "Tutorial: Creating an LLVM Backend for the Cpu0 
Architecture" as follows.

.. code-block:: console

  1-160-136-173:tools Jonathan$ pwd
  /Users/Jonathan/llvm/test/src/tools
  1-160-136-173:tools Jonathan$ ls
  ...
  lld              llvm-config      llvm-extract    llvm-nm           llvm-stress   obj2yaml

Next, setup Cpu0 backend as follows,

.. code-block:: console

  1-160-136-173:lld Jonathan$ pwd
  /Users/Jonathan/llvm/test/src/tools/lld
  1-160-136-173:lld Jonathan$ cp -rf ~/test/lbt/exlbt/lld/* .

Now, build lld with Cpu0 backend as follows,

.. code-block:: console

  1-160-136-173:cmake_debug_build Jonathan$ cmake -DCMAKE_CXX_COMPILER=clang++ -
  DCMAKE_C_COMPILER=clang -DCMAKE_CXX_FLAGS=-std=c++11 -DCMAKE_BUILD_TYPE=Debug
  -G "Xcode" ../src
  ...
  -- Targeting Cpu0
  ...
  -- Configuring done
  -- Generating done
  -- Build files have been written to: /Users/Jonathan/llvm/test/cmake_debug_build

If using VM (guest machine is Linux) or Linux, build as follows,

.. code-block:: console

  [Gamma@localhost cmake_debug_build]$ cmake -DCMAKE_CXX_COMPILER=g++ -
  DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_FLAGS=-std=c++11 -DCMAKE_BUILD_TYPE=Debug
  -G "Unix Makefiles" ../src
  ...
  -- Targeting Cpu0
  ...
  -- Configuring done
  -- Generating done
  -- Build files have been written to: /home/cschen/llvm/test/cmake_debug_build


LLD introduction 
~~~~~~~~~~~~~~~~

In general, linker do the Relocation Records Resolve as Chapter ELF support 
depicted, and optimization for those cannot finish in compiler stage. One of 
the optimization opportunities in linker is Dead Code Stripping which is 
explained in this section. 

List the LLD project status as follows,

- The lld project aims to to be the built-in linker for clang/llvm.
  Currently, clang must invoke the system linker to produce executables.

- web site http://lld.llvm.org/

- Current Status

  - lld is in its early stages of development.
  - It can currently self host on Linux x86-64 with -static.

- How to build

  - cmake -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_FLAGS=-std
    =c++11 -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" ../src/


This whole book focuses on backend design, and this chapter is same. 
To help readers 
understand the lld document, first we list the linking steps from lld web. 
After that, explain each step with the class of source code and what kind of 
Cpu0 backend implementation needed in each step. 
Since some of the following come from our understanding,
please read the lld design web document first (only a few pages), 
http://lld.llvm.org/design.html, then reading the following to 
ensure you agree to our understanding.


How LLD do the linker job
`````````````````````````

- LLD structure

  - Internal structure Atom

    - Like llvm IR, lld operating and optimize in Atom.

  - ELF reader/writer, Mach-O reader/writer, COFF

    - Connect to any specific linker format by implement the concrete Read/Writer.

    - e.g. Implement Microsoft link format Reader/Writer
      => extend lld to support Microsoft link format.

- Atom

  - An atom is an indivisible chunk of code or data.

  - Typically each user written function or global variable is an atom.

  - In addition, the compiler may emit other atoms, such as for literal c-strings 
    or floating point constants, or for runtime data structures like dwarf unwind 
    info or pointers to initializers.

- Atoms classified:

  - The following Hello World code can be classified with these different kinds of 
    Atoms as follows,

  .. rubric:: Atom example code
  .. code-block:: c++

    extern int printf(const char *format, ...);

    int main(void)
    {
      char *ptr = "Hello world!";

      printf("%s\n", ptr);
    }

  - DefinedAtom

    - 95% of all atoms. This is a chunk of code or data

  - UndefinedAtom

    - printf in this example.

  - SharedLibraryAtom

    - Symbols defined in shared library (file \*.so).

  - AbsoluteAtom

    - This is for embedded support where some stuff is implemented in ROM at some 
      fixed address.

.. _lld-atom: 
.. figure:: ../Fig/lld/atom.png
  :scale: 100 %
  :align: center

  Atom classified (from lld web)


Linking Steps
`````````````

- Command line processing

  - lld -flavor gnu hello.o printf-stdarg.o -o a.out

- Parsing input files

  - ELF reader => create lld:File

- Resolving

  - dead code stripping

- Passes/Optimizations

  - Like llvm passes, give backend a chance to do something like optimization. 

- Generate output file

  - Resolving Relocation Records – I guess in this step

Command line processing 
'''''''''''''''''''''''

To support a new backend, refer to Cpu0 code added above.

Parsing input files 
'''''''''''''''''''

- Input Files

  - A goal of lld is to be file format independent.

  - The lld::Reader is the base class for all object file readers

  - Every Reader subclass defines its own “options” class (for instance the 
    mach-o Reader defines the class ReaderOptionsMachO). This options class is 
    the one-and-only way to control how the Reader operates when parsing an input 
    file into an Atom graph

- Reader

  - Refer lld/lib/ReaderWriter/ELF/Reader.cpp and it's related files since lld 
    change quickly.


- lld::File representations

  - In memory, abstract C++ classes (lld::Atom, lld::Reference, and lld::File).

    - Data structure keeped in memory to be fast

  - textual (in YAML)

    - target-triple:   x86_64-apple-darwin11

    - atoms:

      - name:    _main

      - scope:   global

      - type:    code

      - content: [ 55, 48, 89, e5, 48, 8d, 3d, 00, 00, 00, 00, 30, c0, e8, 00, 00,
        00, 00, 31, c0, 5d, c3 ]

  - binary format (“native”)

    - With this model for the native file format, files can be read and turned 
      into the in-memory graph of lld::Atoms with just a few memory allocations. 
      And the format can easily adapt over time to new features.


Resolving
'''''''''

- Dead code stripping (if requested) is done at the end of resolving. 

- The linker does a simple mark-and-sweep. It starts with **“root”** atoms (like 
  “main” in a main executable) and follows each references and marks each Atom 
  that it visits as **“live”**. 

- When done, all atoms not marked **“live”** are removed.

.. rubric:: Dead code stripping - example (modified from llvm lto document web)

.. rubric:: a.h
.. literalinclude:: ../exlbt/input/a.h

.. rubric:: a.cpp
.. literalinclude:: ../exlbt/input/a.cpp

.. rubric:: ch13_1.cpp
.. literalinclude:: ../exlbt/input/ch13_1.cpp

Above code can be reduced to :numref:`lld-deadcodestripping` to perform
mark and swip in graph for Dead Code Stripping.

.. _lld-deadcodestripping: 
.. figure:: ../Fig/lld/deadcodestripping.png
  :scale: 40 %
  :align: center

  Atom classified (from lld web)


As above example, the foo2() is an isolated node without any reference. It's 
dead code and can be removed in linker optimization. We test this example by 
build-ch13_1.sh and find foo2() cannot be removed. 
There are two possibilities for this situation. 
One is we do not trigger lld dead code stripping 
optimization in command (the default is not do it). The other is lld hasn't 
implemented it yet at this point. It's reasonable since the 
lld is in its early stages of development. We didn't dig it more, since the 
Cpu0 backend tutorial just need a linker to finish Relocation Records Resolve 
and see how it runs on PC.

Remind, llvm-linker is the linker works on IR level linker optimization. 
Sometime when you got the obj file only (if you have a.o in this case), 
the native linker (such as lld) have the opportunity to do Dead Code Stripping 
while the IR linker hasn't.


Passes/Optimizations
''''''''''''''''''''

- Passes

  - stub (PLT) generation

  - GOT instantiation

  - order_file optimization

  - branch island generation

  - branch shim generation

  - Objective-C optimizations (Darwin specific)

  - TLV instantiation (Darwin specific)

  - DTrace probe processing (Darwin specific)

  - compact unwind encoding (Darwin specific)


Generate Output File
''''''''''''''''''''

- All concrete writers (e.g. ELF, mach-o, etc) are subclasses of the lld::Writer 
  class.

- Every Writer subclass defines its own “options” class (for instance the mach-o 
  Writer defines the class WriterOptionsMachO). This options class is the 
  one-and-only way to control how the Writer operates when producing an output 
  file from an Atom graph.


For the following example code run, the book example code, exlbt.tar.gz, 
untared in directory /Users/Jonathan/test/lbt/. 
The Cpu0 backend code, lbdex.tar.gz, untared in
the same directory too. The lbdex.tar.gz can be get from the bottom of web, 
http://jonathan2251.github.io/lbd/index.html.

.. code-block:: console

  1-160-136-173:input Jonathan$ pwd
  /Users/Jonathan/test/lbt/exlbt/input
  1-160-136-173:input Jonathan$ ls ../..
  ... exlbt ... lbdex ...
  /Users/Jonathan/test/lbt/exlbt/input
  1-160-136-173:input Jonathan$ bash build-hello.sh cpu032I be
  1-160-136-173:input Jonathan$ /Users/Jonathan/llvm/test/cmake_debug_build/
  Debug/bin/llvm-objdump -s a.out
  ...                .
  Contents of section .plt:
   0140 3600000c 36000004 36000004 36fffffc  6...6...6...6...
  Contents of section .text:
   0150 09ddfff8 02ed0004 02cd0000 11cd0000  ................
  ...
  Contents of section .rodata:
   0b98 286e756c 6c290048 656c6c6f 20776f72  (null).Hello wor
   0ba8 6c642100 25730a00                    ld!.%s..


Next section will show you how to design your lld backend and register a pass 
for Relocation Records Solve in details through Cpu0 backend code explantation.


Static linker 
~~~~~~~~~~~~~

Let's run the static linker first and explain it next.

Run
```

File printf-stdarg.c come from internet download which is GPL2 license. GPL2 
is more restricted than LLVM license. 
File printf-stdarg-1.c is the file for testing the printf() function which
implemented on PC OS platform. Let's run printf-stdarg-2.cpp on Cpu0 and
compare it against the result of PC's printf() as below.

.. rubric:: exlbt/input/printf-stdarg-1.c
.. literalinclude:: ../exlbt/input/printf-stdarg-1.c
    :start-after: /// start

.. rubric:: exlbt/input/printf-stdarg-2.cpp
.. literalinclude:: ../exlbt/input/printf-stdarg-2.cpp
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

.. rubric:: exlbt/input/lib_cpu0.ll
.. literalinclude:: ../exlbt/input/lib_cpu0.ll
    :start-after: /// start

.. rubric:: exlbt/input/functions.sh
.. literalinclude:: ../exlbt/input/functions.sh

.. rubric:: exlbt/input/build-printf-stdarg-2.sh
.. literalinclude:: ../exlbt/input/build-printf-stdarg-2.sh

.. rubric:: lbdex/verilog/Makefile
.. literalinclude:: ../lbdex/verilog/Makefile


The build-printf-stdarg-2.sh is for my PC setting. Please change this script to
the directory of your llvm/lld setting. After that run static linker example 
code as follows,

.. code-block:: console

  1-160-136-173:input Jonathan$ pwd
  /Users/Jonathan/test/lbt/exlbt/input
  1-160-136-173:input Jonathan$ bash build-printf-stdarg-2.sh cpu032I be
  In file included from printf-stdarg-2.cpp:11:
  ./printf-stdarg.c:206:15: warning: conversion from string literal to 'char *' 
  is deprecated [-Wdeprecated-writable-strings]
    char *ptr = "Hello world!";
                ^
  1 warning generated.
  
  1-160-136-173:input Jonathan$ cd ../../lbdex/verilog/
  1-160-136-173:verilog Jonathan$ pwd
  /Users/Jonathan/test/lbt/lbdex/verilog
  1-160-136-173:verilog Jonathan$ make
  1-160-136-173:verilog Jonathan$ ls
  ... cpu0Isp ... cpu0IIsp ...
  1-160-136-173:verilog Jonathan$ ./cpu0Isp
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

Let's check the result with PC program printf-stdarg-1.c output as follows,

.. code-block:: console

  1-160-136-173:input Jonathan$ clang printf-stdarg-1.c
  printf-stdarg-1.c:58:19: warning: incomplete format specifier [-Wformat]
    printf("%d %s(s)%", 0, "message");
                    ^
  1 warning generated.
  1-160-136-173:input Jonathan$ ./a.out
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

They are same. You can verify the slt instructions is work fine too by change 
variable cpu from cpu032I to cpu032II as follows,

.. rubric:: exlbt/input/build-printf-stdarg-2.sh


.. code-block:: console

  1-160-136-173:verilog Jonathan$ pwd
  /Users/Jonathan/test/lbt/lbdex/verilog
  1-160-136-173:verilog Jonathan$ cd ../../exlbt/input
  1-160-136-173:input Jonathan$ pwd
  /Users/Jonathan/test/lbt/exlbt/input
  1-160-136-173:input Jonathan$ bash build-printf-stdarg-2.sh cpu032II be
  ...
  1-160-136-173:input Jonathan$ cd ../lbdex/verilog/
  1-160-136-173:verilog Jonathan$ ./cpu0IIsp

The verilog machine cpu0IIs include all instructions of cpu032I and add 
slt, beq, ..., instructions.
Run build-printf-stdarg-2.sh with cpu=cpu032II will generate slt, beq and bne 
instructions instead of cmp, jeq, ... instructions.

With the printf() of GPL source code, we can program more test code with it 
to verify the previous llvm Cpu0 backend generated program. The following code 
is for this purpose.

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

.. rubric:: exlbt/input/build-slinker.sh
.. literalinclude:: ../exlbt/input/build-slinker.sh
  
.. code-block:: console

  1-160-136-173:input Jonathan$ pwd
  /Users/Jonathan/test/lbt/exlbt/input
  114-37-148-111:input Jonathan$ bash build-slinker.sh cpu032I le
  ...
  In file included from ch_slinker.cpp:23:
  ./ch_lld_staticlink.cpp:8:15: warning: conversion from string literal to 
  'char *' is deprecated
        [-Wdeprecated-writable-strings]
    char *ptr = "Hello world!";
                ^
  1 warning generated.
  114-37-148-111:input Jonathan$ cd ../../lbdex/verilog/
  114-37-148-111:verilog Jonathan$ ./cpu0IIsp
  WARNING: ./cpu0.v:369: $readmemh(cpu0.hex): Not enough words in the file for 
  the requested range [0:524287].
  taskInterrupt(001)
  ...
  test_nolld(): PASS
  taskInterrupt(011)
  Overflow exception
  taskInterrupt(011)
  Overflow exception
  test_overflow = 0, PASS
  global variable gI = 100, PASS
  test_ctrl2(): a = 1, b = 0, c = 1, d = 0, PASS
  test_phinode(3, 1) = 3, PASS
  test_blockaddress(1) = 1, PASS
  test_blockaddress(2) = 2, PASS
  date1 = 2012 10 12 1 2 3, PASS
  date2 = 2012 10 12 1 2 3, PASS
  time2 = 1 10 12, PASS
  time3 = 1 10 12, PASS
  date1 = 2013 1 26 12 21 10, PASS
  date2 = 2013 1 26 12 21 10, PASS
  test_template() = 15, PASS
  test_alloc() = 31, PASS
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

As above, by taking the open source code advantage, Cpu0 got the more stable 
printf() program. 
Once Cpu0 backend can translate the printf() function of the open source C 
printf() program into machine instructions, the llvm Cpu0 backend can be 
verified with printf(). 
With the quality code of open source printf() program, the Cpu0 toolchain is 
extended from compiler backend to C std library support. (Notice that some GPL 
open source code are not quality code, but some are.)

The "Overflow exception is printed twice meaning the ISR() of debug.cpp is 
called twice from ch4_1_2.cpp. 
The printed "taskInterrupt(001)" and "taskInterrupt(011)" just are trace 
message from cpu0.v code.


Dynamic linker 
~~~~~~~~~~~~~~

I remove dynamic linker demostration from 3.9.0 because I don't know how to
do it from lld 3.9 and this demostration add lots of code in elf2hex, verilog 
and lld of Cpu0 backend. However it can be run with llvm 3.7 with the following 
command.

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

Then reading this section in lld.html for it.


Summary
--------

Create a new backend base on LLVM
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Thanks the llvm open source project. 
To write a linker and ELF to Hex tools for a new CPU architecture is easy and 
reliable. 
Combined with the llvm Cpu0 backend code and Verilog language code programmed
in previouse chapters, we design a software 
toolchain to compile C/C++ code, link and run it on Verilog Cpu0 simulator 
without any real hardware investment.
If you buy the FPGA development hardware, we believe these 
code can run on FPGA CPU even though we didn't do it.
Extend system program toolchain to support a new CPU instruction set can be 
finished just like we have shown you at this point. 
School knowledges of system program, compiler, linker, loader, computer 
architecture and CPU design has been translated into a real work and see how it 
is running. Now, these school books knowledge is not limited on paper. 
We design it, program it, and run it on real world.

The total code size of llvm Cpu0 backend compiler, Cpu0 lld linker, elf2hex and 
Cpu0 Verilog Language is around 10 thousands lines of source code include 
comments. 
The total code size of clang, llvm and lld has 1000 thousands lines exclude the
test and documents parts. It is only 1 \% of the llvm size.
More over, the llvm Cpu0 backend and lld Cpu0 backend are 70% of same with llvm 
Mips and lld X86_64.
Based on this truth, we believe llvm is a well defined structure in compiler 
architecture. 

Contribute back to Open Source through working and learning
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Finally, 10 thousands lines of source code in Cpu0 backend is very small in UI 
program. But it's quite complex in system program which based on llvm. 
We spent 600 pages of pdf to explain these code. Open source code give 
programmers best opportunity to understand the code and enhance/extend the 
code function. But it can be better, we believe the documentation is the next 
most important thing to improve the open source code development. 
The Open Source Organization recognized this point and set 
Open Source Document Project years ago [#BSDLicense]_ [#docproj]_ 
[#freebsdLicense]_ [#gnuLicense]_ [#fdl]_.
Open Source grows up and becomes a giant software infrastructure with the forces 
of company [#apple-opensource]_  [#ibm-opensource]_, school research team and 
countless talent engineers passion. 
It terminated the situation of everyone trying to re-invent wheels during 10 
years ago.
Extend your software from the re-usable source code is the right way. 
Of course you should consider an open source license if you are working 
with business.
Actually anyone can contribute back to open source through the learning process. 
This book is written through the process of learning llvm backend and contribute 
back to llvm open source project.
We think this book cannot exists in traditional paper book form since only 
few number of readers interested in study llvm backend even though
there are many paper published books in concept of compiler. So, this book 
is published via electric media form and try to match the Open Document License 
Expection [#gnu-phi]_.
There are distance between the concept and the realistic program implemenation. 
Keep note through learning a large complicate software such as this llvm backend 
is not enough. 
We all learned the knowledge through books during school and after school. 
So, if you cannot find a good way to produce documents, you can consider to 
write documents like this book. This book document uses sphinx tool 
just like the llvm development team. Sphinx uses restructured text format here
[#rst-ref]_ [#rst-directives]_ [#rst]_.
Appendix A of lbd book tell you how to install sphinx tool. 
Documentation work will help yourself to re-examine your software and make your 
program better in structure, reliability and more important "Extend your code 
to somewhere you didn't expect".


.. [#lldweb] http://lld.llvm.org/

.. [#lld-install] http://lld.llvm.org/getting_started.html#on-unix-like-systems

.. [#llvm-download] http://llvm.org/releases/download.html#3.5

.. [#effectiveC++] http://www.cplusplus.com/reference/memory/unique_ptr/

.. [#get] http://www.cplusplus.com/reference/memory/unique_ptr/get/

.. [#move] http://www.cplusplus.com/reference/utility/move/

.. [#pic-call-stub] Page 5-12 of http://www.linux-mips.org/pub/linux/mips/doc/ABI/mipsabi.pdf

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

