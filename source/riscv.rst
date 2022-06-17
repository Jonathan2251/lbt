.. _sec-riscv:

RISCV
=====

.. contents::
   :local:
   :depth: 4

This chapter shows the RISC toolchain installatation including gnu, llvm and 
simulators on Linux.


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
  $ $HOME/riscv/git
  $ bash riscv-toolchain-setup.sh

RISCV toolchain includes both baremetal(Newlib) and Linux platform support.

Qemu simulator
--------------

Install according https://gitlab.com/qemu-project/qemu as follows,

.. code-block:: console

  $ pwd
  $ $HOME/riscv/git
  $ git clone https://gitlab.com/qemu-project/qemu.git
  $ mkdir build
  $ cd build
  $ ../configure
  $ make

Then it can compile and run qemu for baremetal as follows,

.. code-block:: console

  $ pwd
  $ $HOME/lbt/exlbt/riscv
  $ $HOME/riscv/riscv_newlib/bin/clang hello.c -o hello_newlib
  $ $HOME/riscv/riscv/git/qemu/build/qemu-riscv64 hello_newlib
  HelloWorld!

  $ $HOME/riscv/riscv_newlib/bin/riscv64-unknown-elf-gcc -c hello_world.s
  $ $HOME/riscv/riscv_newlib/bin/riscv64-unknown-elf-ld hello_world.o -o hello_world
  $ $HOME/riscv/git/qemu/build/qemu-riscv64 hello_world
  Hello World

Linux as follows,

.. code-block:: console

  $ $HOME/riscv/riscv_linux/bin/clang -march=rv64g hello.c -o hello_linux -static
  $ $HOME/riscv/riscv/git/qemu/build/qemu-riscv64 hello_linux
  HelloWorld!


Gem5 simulator
--------------

Build Gem5 according the following,

http://learning.gem5.org/book/part1/building.html#requirements-for-gem5

http://learning.gem5.org/book/part1/example_configs.html


.. code-block:: console

  $ pwd
  $ $HOME/riscv/git/gem5
  $HOME/riscv/git/gem5$ /usr/bin/env python3 $(which scons) ./build/RISCV/gem5.debug -j4
  ...
  $ pwd
  $ $HOME/lbt/exlbt/riscv
  $HOME/lbt/exlbt/riscv$ $HOME/riscv/git/gem5/build/RISCV/gem5.debug $HOME/riscv/git/gem5/configs/example/se.py --cmd=./hello_newlib
  **** REAL SIMULATION ****
  build/RISCV/sim/simulate.cc:107: info: Entering event queue @ 0.  Starting simulation...
  HelloWorld!

Check cycles as follows,

.. code-block:: console

  /local/git$ vi m5out/stats.txt
  simSeconds                                   0.000001                       # Number of seconds simulated (Second)
  simTicks                                      1229000                       # Number of ticks simulated (Tick)
  ...

  $HOME/lbt/exlbt/riscv$ $HOME/riscv/git/gem5/build/RISCV/gem5.debug /local/git/gem5/configs/example/se.py --cmd=./hello_linux
  ...
  $HOME/lbt/exlbt/riscv$ build/RISCV/base/loader/image_file_data.cc:111: panic: panic condition fd < 0 occurred: Failed to open file /lib/ld-linux-riscv64-lp64d.so.1.
  ...

