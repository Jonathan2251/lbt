.. _sec-riscv:

Appendix A: RISCV
=================

.. contents::
   :local:
   :depth: 4

This chapter shows the RISC toolchain installatation including gnu, llvm and 
simulators on Linux.


ISA
---

.. _riscv-f1:
.. figure:: ../Fig/riscv/riscv-isa.png
  :scale: 50 %
  :align: center

  RISCV ISA

.. _riscv-f2:
.. figure:: ../Fig/riscv/isa-desc.png
  :scale: 50 %
  :align: center

  RISCV ISA Description


As above figure, RISC has 32/64/128 bit and I (integer) is the Base part and the 
others are optional [#ISA]_ [#RISCV-wiki]_ [#RRE]_.

RISC compiler toolchain installatation
--------------------------------------

First, install the depended packages according 
https://github.com/riscv-collab/riscv-gnu-toolchain#readme. 
Next, create your $HOME/riscv and $HOME/riscv/git. Then run the following bash
script.

.. rubric:: exlbt/riscv/riscv-toolchain-setup.sh
.. literalinclude:: ../exlbt/riscv/riscv-toolchain-setup.sh

.. code-block:: console

  $ pwd
  $ $HOME/lbt/exlbt/riscv
  $ bash riscv-toolchain-setup.sh

RISCV toolchain includes both baremetal(Newlib) and Linux platform support.

.. code-block:: console

  $ pwd
  $ $HOME/lbt/exlbt/riscv
  $ bash riscv-toolchain-setup.sh

.. code-block:: console

  $ ls ~/riscv/riscv_newlib 
  bin			include			lib			libexec			riscv64-unknown-elf	share

.. _riscv-f3:
.. figure:: ../Fig/riscv/linux-sysroot.png
  :scale: 50 %
  :align: center

  RISCV ISA Description

The linux sysroot as figure above and compare it with the installed dirctory.

.. code-block:: console

  $ ls /local/riscv_linux/sysroot/
  etc  lib  lib64  sbin  usr  var
  $ ls /local/riscv_linux/sysroot/usr
  bin  include  lib  libexec  sbin  share


Qemu simulator
--------------

Install according https://gitlab.com/qemu-project/qemu as follows,

.. code-block:: console

  $ pwd
  $ $HOME/riscv/git
  $ git clone https://gitlab.com/qemu-project/qemu.git
  $ cd qemu
  $ git log
  commit a28498b1f9591e12dcbfdf06dc8f54e15926760e
  ...
  $ mkdir build
  $ cd build
  $ ../configure
  $ make

Then it can compile and run qemu for baremetal as follows,

.. code-block:: console

  $ pwd
  $ $HOME/lbt/exlbt/riscv
  $ $HOME/riscv/riscv_newlib/bin/clang -march=rv64g hello.c -o hello_newlib
  $ $HOME/riscv/git/qemu/build/qemu-riscv64 hello_newlib
  HelloWorld!

  $ $HOME/riscv/riscv_newlib/bin/riscv64-unknown-elf-gcc -c hello_world.s
  $ $HOME/riscv/riscv_newlib/bin/riscv64-unknown-elf-ld hello_world.o -o hello_world
  $ $HOME/riscv/git/qemu/build/qemu-riscv64 hello_world
  Hello World

Linux as follows,

.. code-block:: console

  $ $HOME/riscv/riscv_linux/bin/clang -march=rv64g hello.c -o hello_linux -static
  $ $HOME/riscv/git/qemu/build/qemu-riscv64 hello_linux
  HelloWorld!


Gem5 simulator
--------------

Build Gem5 according the following,

http://learning.gem5.org/book/part1/building.html#requirements-for-gem5

If you don't have python3.x-config on Ubuntu 18.04 as follows,

.. code-block:: console

  $ ls /usr/bin/python*
  ... /usr/bin/python2.7-config

Then do install by pip3 as follows [#install-python3-config]_,

.. code-block:: console

  $ sudo apt install python3-pip
  $ pip3 install scons
  $ ls /usr/bin/python*
  ... /usr/bin/python3-config

After install all dependencies, get gem5 and build RISCV as follows,

.. code-block:: console

  $ pwd
  $ $HOME/riscv/git
  $ sudo apt install -y libglib2.0-dev
  $ sudo apt install -y libpixman-1-dev
  $ git clone https://gem5.googlesource.com/public/gem5
  $ cd gem5
  $ git log
  commit 846dcf0ba4eff824c295f06550b8673ff3f31314
  ...
  $HOME/riscv/git/gem5$ /usr/bin/env python3 $(which scons) ./build/RISCV/gem5.debug -j4
  ...
  $ pwd
  $ $HOME/lbt/exlbt/riscv
  $HOME/lbt/exlbt/riscv$ $HOME/riscv/git/gem5/build/RISCV/gem5.debug \
  $HOME/riscv/git/gem5/configs/example/se.py --cmd=./hello_newlib
  **** REAL SIMULATION ****
  build/RISCV/sim/simulate.cc:107: info: Entering event queue @ 0.  Starting simulation...
  HelloWorld!

Check cycles as follows,

.. code-block:: console

  $HOME/lbt/exlbt/riscv$ vi m5out/stats.txt
  simSeconds                                   0.000001                       # Number of seconds simulated (Second)
  simTicks                                      1229000                       # Number of ticks simulated (Tick)
  ...

  $HOME/lbt/exlbt/riscv$ $HOME/riscv/git/gem5/build/RISCV/gem5.debug \
  /local/git/gem5/configs/example/se.py --cmd=./hello_linux
  ...
  HelloWorld!
  ...

The config of gem5 reference here.
http://learning.gem5.org/book/part1/example_configs.html


RVV
----

Clang/llvm provide RVV (RISC-V Vectors) written in C rather than inline-asm. 
Though it notices as clang option: -target-feature +experimental-v, this way in 
C is shorter, more user-friendly and easy to remember for users than in 
inline-asm form. 
Builtin is C function and friendly either. RVV can be written and run as follows,

.. rubric:: exlbt/riscv/vadd2.c
.. literalinclude:: ../exlbt/riscv/vadd2.c

.. code-block:: console

  $ pwd
  $ $HOME/lbt/exlbt/riscv
  $ $HOME/riscv/riscv_newlib/bin/clang vadd2.c -menable-experimental-extensions \
    -march=rv64gcv0p10 -O0 -mllvm --riscv-v-vector-bits-min=256
  $ $HOME/riscv/git/qemu/build/qemu-riscv64 -cpu rv64,v=true a.out
  vector version is not specified, use the default value v1.0
  array_size(a):4096

  a[]: 0 2 4 6 8 10 12 14 16 18 ...
  The results of vadd:	PASS

  a[]: 0 4 8 12 16 20 24 28 32 36 ...
  The results of vadd_asm:	PASS

  $ $HOME/riscv/riscv_newlib/bin/clang vadd1.c -menable-experimental-extensions \
    -march=rv64gcv0p10 -O0 -mllvm --riscv-v-vector-bits-min=256 -static
  $ $HOME/riscv/git/qemu/build/qemu-riscv64 -cpu rv64,v=true a.out
  vector version is not specified, use the default value v1.0
  1 11 11 11 11 11 11 11 11 1 
  $ $HOME/riscv/riscv_newlib/bin/riscv64-unknown-elf-objdump -d a.out|grep vadd.vv
   106fc:	03ae0d57          	vadd.vv	v26,v26,v28

Clang/llvm provide builtin and intrinsic functions to implement RVV (RISC-V 
Vectors) 

.. rubric:: ~/riscv/git/llvm-project/clang/include/clang/Basic/riscv_vector.td
.. code-block:: c++

  #define vsetvl_e8mf8(avl) __builtin_rvv_vsetvli((size_t)(avl), 0, 5)
  ...
  defm vmadd  : RVVIntTerBuiltinSet;
  ...
  defm vfdiv  : RVVFloatingBinBuiltinSet;

.. rubric:: ~/riscv/git/llvm-project/llvm/include/llvm/IR/IntrinsicsRISCV.td
.. code-block:: c++

   def int_riscv_vsetvli   : Intrinsic<...
   ...
   defm vmadd : RISCVTernaryAAXA;
   ...
   defm vfdiv : RISCVBinaryAAX;

Refer to clang/llvm test cases in the following folders.

.. code-block:: console

  $ ~/riscv/git/llvm-project/clang/test/CodeGen/RISCV/rvv-intrinsics$ ls
  $ ... vfdiv.c ... vfmadd.c

  $ ~/riscv/git/llvm-project/llvm/test/CodeGen/RISCV/rvv$ ls
  $ ... vfdiv-rv64.ll ... vfmadd-rv64.ll ...


.. [#ISA] https://riscv.org/technical/specifications/

.. [#RISCV-wiki] https://en.wikipedia.org/wiki/RISC-V

.. [#RRE] https://wiki.riscv.org/display/HOME/Recently+Ratified+Extensions

.. [#install-python3-config] https://www.anycodings.com/questions/gem5-build-fails-with-embedded-python-library-36-or-newer-required-found-2717