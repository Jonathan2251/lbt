.. _sec-pocl:

Appendix C: POCL
=================

.. contents::
   :local:
   :depth: 4

.. _opencl-f:
.. graphviz:: ../Fig/pocl/opencl.gv
  :caption: OpenCL with discrete memory (Device: 2 processor+memory)

As :numref:`opencl-f`, one possible HW platform has one Host and Device with
2 RISCV processors, each with dedicated memory.

- info::local_mem_type::global -> coherent, CPU cache (SRAM)
- info::local_mem_type::local -> dedicated local memory, GPU cache (SRAM)

OpenCL includes three components: Runtime API, Driver, and Compiler.
POCL is an open-source implementation of the Runtime API.

OpenCL Implementation [#ocl-impl]_.
Wiki OpenCL Open Source Implementations [#wiki-ocl-osi]_.

OpenCL
------

Builtin-function and builtin-kernel reference here [#builtin-function-kernel]_.
Books [#open-cl-book-heter]_.
Papers [#ocl-acceler-tf]_ [#ocl-agpvldl]_ [#sycl-aesstgplsmlw]_ [#ocl-acpcco]_
[#dlc-acs]_ in lbt/papers/pocl.

Run OpenCL
----------

MacOS
*****

You can create the project ~/git/lbt/exlbt/opencl/opencl-saxpy/opencl-saxpy.xcodeproj by:

- Open Xcode
- Create a new Xcode project
- Select macOS → Command Line Tool
- Set Product Name: opencl-saxpy, Language: C
- Click Next → Create
- Choose folder ~/git/lbt/exlbt/opencl
- On the left, in the opencl-saxpy folder, delete the default main.c → Move to Trash
- Copy saxpy.c into ~/git/lbt/exlbt/opencl/opencl-saxpy/opencl-saxpy
- On the left, right-click opencl-saxpy folder → Add Files to "opencl-saxpy"
- Save the project

Build and run the saxpy OpenCL program:

- Open ~/git/lbt/exlbt/opencl/opencl-saxpy/opencl-saxpy.xcodeproj
- Choose Product → Build
- Choose Product → Run

Refer to the icons shown in :numref:`pocl-f1` to see compile options and run results.

.. _pocl-f1:
.. figure:: ../Fig/pocl/opencl-saxpy-build-run.png
  :scale: 50 %
  :align: center

  Build and run opencl-saxpy.xcodeproj


Open sources
------------

.. table:: Open sources for OpenCL

  ==============  =======  ======  ========  =======
  Project         Runtime  Driver  Compiler  Library
  ==============  =======  ======  ========  =======
  POCL            V        V     
  clang                            V
  libclc                                     V
  ==============  =======  ======  ========  =======

.. table:: Open sources for OpenCL 2

  ==============  ==============  ===================  ===========
  project         Host Compiler   Device Compiler      Device Lib
  ==============  ==============  ===================  ===========
  POCL            X86             X86,ARM,AMD,TCE,PTX  
  clang           X86             NVIDIA-gpu, AMD-gpu  
  libclc                                               Spir, Spir64, NVIDIA-gpu, AMD-gpu
  ==============  ==============  ===================  ===========

libclc is an open source, BSD/MIT dual licensed implementation of 
the library requirements of the OpenCL C programming language [#libclc]_.

POCL
----

Pocl web [#pocl-web]_ and documentation [#pocl-doc]_.

PoCL uses Clang as an OpenCL C frontend and LLVM for kernel compiler 
implementation, and as a portability layer. Thus, if your desired target has an 
LLVM backend, it should be able to get OpenCL support easily by using PoCL 
[#pocl-web]_.

-  PoCL is an open-source, CPU-focused OpenCL implementation.
-  It supports:

   - x86 CPUs (including AMD CPUs)
   - Some experimental GPU backends

.. note:: OpenCL is a C-based language with the keyword `__kernel` and special 
          attributes in data types. If Clang can compile `input.cl` to LLVM IR 
          (`input.ll`), then the LLVM backend can compile `input.ll` and link 
          it with libraries.

Build as the following bash:

.. rubric:: exlbt/pocl/pocl-install.sh
.. literalinclude:: ../exlbt/pocl/pocl-install.sh

Add apt repo on Ubuntu at first time,

.. code-block:: console

  $ sudo apt-add-repository 'deb https://apt.llvm.org/bionic/   llvm-toolchain-bionic-13 main'
  $ grep "apt.llvm" /etc/apt/sources.list /etc/apt/sources.list.d/*
  /etc/apt/sources.list:deb http://apt.llvm.org/bionic/ llvm-toolchain-bionic-13 main
  /etc/apt/sources.list:# deb-src http://apt.llvm.org/bionic/ llvm-toolchain-bionic-13 main
  /etc/apt/sources.list:deb https://apt.llvm.org/bionic/ llvm-toolchain-bionic-13 main
  /etc/apt/sources.list:# deb-src https://apt.llvm.org/bionic/ llvm-toolchain-bionic-13 main


Reference [#pocl-build]_.



Structure
---------

The runtime code is located in `pocl/lib/CL`. Test cases for the runtime can be
found in `pocl/tests/runtime`.

GDB on POCL
-----------

After running `bash pocl-install.sh`, you can execute `make check_tier1`. However,
running a single test such as `./tests/runtime/test_clCreateKernelsInProgram`
fails, as shown below:

.. code-block:: console

  ~/git/pocl/build$ POCL_DEBUG=all ./tests/runtime/test_clCreateKernelsInProgram
  ...
  [2022-07-15 08:43:44.140733258219579]POCL: in fn pocl_init_devices at line 529:
    |   WARNING |  cschen:Loading 
    /home/cschen/git/pocl/build/lib/CL/pocl/libpocl-devices-basic.so failed: 
    /home/cschen/git/pocl/build/lib/CL/pocl/libpocl-devices-basic.so: cannot 
    open shared object file: No such file or directory
  ...
  [2022-10-06 08:48:06.140725365544843]POCL: in fn pocl_init_devices at line 584:
    |     ERROR | CL_DEVICE_NOT_FOUND no devices found. POCL_DEVICES=(null)
  CL_DEVICE_NOT_FOUND in poclu_get_any_device on line 22

Failing to run a single test executable is not acceptable for tracing the
pocl code. A workaround fix is as follows:

.. code-block:: console

  ~/git/pocl/build$ touch ../tests/runtime/test_clCreateKernelsInProgram.c
  ~/git/pocl/build$ make VERBOSE=1|grep test_clCreateKernelsInProgram
  ...
  /usr/lib/llvm-13/bin/clang -g  -pie 
  CMakeFiles/test_clCreateKernelsInProgram.dir/test_clCreateKernelsInProgram.c.o 
  -o test_clCreateKernelsInProgram  -Wl,-rpath,/home/cschen/git/pocl/build/lib/CL 
  ../../poclu/libpoclu.a ../../lib/CL/libOpenCL.so.2.9.0 
  -L/usr/lib/x86_64-linux-gnu -lhwloc /usr/lib/llvm-13/lib/libclang-cpp.so 
  /usr/lib/llvm-13/lib/libLLVM-13.so -lrt -lm -ldl -lm -ldl 

Change the link path (`rpath`) from
`/home/cschen/git/pocl/build/lib/CL` to `/usr/local/lib`
for `libpocl-devices-basic.so`, then it passes as follows:

.. code-block:: console

  ~/git/pocl/build$ cd /home/jonathanchen/git/pocl/build/tests/runtime
  ~/git/pocl/build/tests/runtime$ /usr/lib/llvm-13/bin/clang -g  -pie 
  CMakeFiles/test_clCreateKernelsInProgram.dir/test_clCreateKernelsInProgram.c.o 
  -o test_clCreateKernelsInProgram  -Wl,-rpath,/usr/local/lib 
  ../../poclu/libpoclu.a ../../lib/CL/libOpenCL.so.2.9.0 -lhwloc 
  /usr/lib/llvm-13/lib/libclang-cpp.so /usr/lib/llvm-13/lib/libLLVM-13.so -lrt 
  -lm -ldl -lm -pthread -ldl
  ~/git/pocl/build/tests/runtime$ cd ../..
  ~/git/pocl/build$ ./tests/runtime/test_clCreateKernelsInProgram
  Hello
  World

Then I can use GDB as follows:

.. code-block:: console

  ~/git/pocl/build/$ gdb --args ./tests/runtime/test_clCreateKernelsInProgram
  (gdb) b test_clCreateKernelsInProgram.c:21
  Breakpoint 1 at 0x23b4: file 
  /home/cschen/git/pocl/tests/runtime/test_clCreateKernelsInProgram.c, line 21.
  (gdb) r
  ...
  Breakpoint 1, main (argc=1, argv=0x7fffffffdfa8) at 
  /home/cschen/git/pocl/tests/runtime/test_clCreateKernelsInProgram.c:23
  21        err = poclu_get_any_device(&ctx, &did, &queue);


Examples of Compiling and Running on POCL
-----------------------------------------

As traced from Clang compilation options in the last section, I added a
`compile.sh` script to run OpenCL programs on POCL as follows:

.. rubric:: exlbt/pocl/ex/compile.sh
.. literalinclude:: ../exlbt/pocl/ex/compile.sh

.. code-block:: console

  ~/git/lbt/exlbt/pocl/ex$ POCL_DEBUG=err,warn
  ~/git/lbt/exlbt/pocl/ex$ bash compile.sh
  ~/git/lbt/exlbt/pocl/ex$ ./a.out
    ** Final POCL_DEBUG flags: 18000000000 
    Hello World!
  ~/git/lbt/exlbt/pocl/ex$ POCL_DEBUG=
  ~/git/lbt/exlbt/pocl/ex$ ./a.out 
    Hello World!

References [#pocl-dbg]_.


RISCV OpenCL
------------

In LLVM, both MIPS and Cpu0 can compile `.ll` files generated by Clang from
OpenCL input. However, RISCV fails to do so on both LLVM 14.x and 15.x, as
shown below:

.. code-block:: console

  CodeGenOpenCL % pwd
  $HOME/git/lbt/exlbt/pocl
  % ~/llvm/14.x/llvm-project/build/bin/clang -cc1 -cl-std=CL2.0 -emit-llvm 
    -triple spir-unknown-unknown test.cl
  % ~/llvm/debug/build/bin/llc -march=mips test.ll
  % ~/llvm/test/build/bin/llc -march=cpu0 test.ll
  % ~/llvm/14.x/llvm-project/build/bin/llc -mtriple=riscv64 test.ll
    LLVM ERROR: Unsupported calling convention
    ...
  % ~/llvm/15.x/llvm-project/build/bin/clang -cc1 -cl-std=CL2.0 -emit-llvm -triple spir-unknown-unknown test.cl
  % ~/llvm/15.x/llvm-project/build/bin/llc -mtriple=riscv64 test.ll
    LLVM ERROR: Unsupported calling convention
    ...

  CodeGenOpenCL % pwd
  $HOME/llvm/14.x/llvm-project/clang/test/CodeGenOpenCL
  CodeGenOpenCL % ~/llvm/debug/build/bin/clang -cc1 -cl-std=CL2.0 -emit-llvm 
                 -triple spir-unknown-unknown overload.cl
  CodeGenOpenCL % ~/llvm/debug/build/bin/llc -march=mips overload.ll
  CodeGenOpenCL % ~/llvm/test/build/bin/llc -march=cpu0 overload.ll
  CodeGenOpenCL % ~/llvm/14.x/llvm-project/build/bin/llc -mtriple=riscv64 overload.ll
                  LLVM ERROR: Unsupported calling convention
                  ...


SYCL
----

Webs [#sycl]_. An example [#sycl-wiki]_.


DPC++
-----

Book [#dpcpp-book]_. 

.. rubric:: exlbt/pocl/dpc++.txt


Why Is CUDA More Popular and Marketable Than OpenCL?
----------------------------------------------------

**CUDA vs OpenCL: Feature Comparison**

+------------------------+------------------------------+-----------------------------+---------------------------------+
| Feature                | CUDA (NVIDIA)                | OpenCL (Khronos Group)      | Market Impact                   |
+========================+==============================+=============================+=================================+
| Language Model         | C++-like, high-level         | C-like, low-level           | ✅ CUDA easier to adopt         |
+------------------------+------------------------------+-----------------------------+---------------------------------+
| Hardware Integration   | NVIDIA-only, tightly coupled | Multi-vendor, generic       | ✅ CUDA better optimized        |
+------------------------+------------------------------+-----------------------------+---------------------------------+
| Ecosystem Libraries    | cuBLAS, cuDNN, TensorRT      | clBLAS, clDNN (less mature) | ✅ CUDA more complete           |
+------------------------+------------------------------+-----------------------------+---------------------------------+
| Tooling & Profiling    | Nsight suite                 | Varies by vendor            | ✅ CUDA more developer-friendly |
+------------------------+------------------------------+-----------------------------+---------------------------------+
| Framework Support      | PyTorch, TensorFlow, JAX     | Limited, indirect support   | ✅ CUDA deeply integrated       |
+------------------------+------------------------------+-----------------------------+---------------------------------+
| Portability            | NVIDIA-only                  | Cross-platform              | ❌ OpenCL more portable         |
+------------------------+------------------------------+-----------------------------+---------------------------------+
| Performance Consistency| High (on NVIDIA GPUs)        | Variable across vendors     | ✅ CUDA more predictable        |
+------------------------+------------------------------+-----------------------------+---------------------------------+

- CUDA supports **full C++** on the device while OpenCL uses **C** in the device side.
- **CUDA "AI-first"; OpenCL general computation with less momentum in deep learning.**
- **iPhone vs Android** situation mirrors CUDA vs OpenCL in many ways.

Legend:

  ✅ = Advantage
  ❌ = Trade-off

CUDA, despite being proprietary, has become the dominant GPU compute platform, 
far surpassing OpenCL in popularity, performance, and ecosystem integration. 
This success is rooted in strategic design choices, developer engagement, and 
tight hardware-software co-design.

1. Early Developer Adoption

CUDA was introduced by NVIDIA in 2006, years before OpenCL gained traction. It 
provided a dedicated, well-documented, and performant programming model for 
GPU compute, attracting researchers, HPC developers, and AI practitioners early 
on.

2. Unified Hardware-Software Stack

CUDA is tightly coupled with NVIDIA’s GPU architectures. Each CUDA release is 
optimized for specific hardware generations (e.g., Volta, Ampere, Hopper), 
ensuring maximum performance.
OpenCL, by contrast, must support multiple vendors and devices, often leading 
to inconsistent performance and fragmented tooling.

3. Rich Ecosystem and Tooling

NVIDIA built a comprehensive ecosystem around CUDA, including:

- cuBLAS, cuDNN, and TensorRT for optimized libraries
- Nsight Systems and Nsight Compute for profiling and debugging
- Deep integration with PyTorch, TensorFlow, and JAX

OpenCL lacks such a unified stack, and its vendor-specific implementations vary 
widely in quality.

4. Developer Experience

CUDA offers a clean, C++-like syntax with powerful abstractions for memory 
management, kernel launches, and parallelism. 
Its documentation, tutorials, and community support are extensive. 
OpenCL, while flexible, is lower-level and more verbose, making it harder to 
learn and maintain.

5. Market Perception and Branding

NVIDIA has positioned **CUDA** as the backbone of modern AI and HPC. 
Its branding as an **“AI-first” company** has made CUDA synonymous with 
cutting-edge innovation. 
**OpenCL**, governed by the Khronos Group, is seen as a **generic standard with 
less momentum in deep learning** and commercial applications.


.. [#ocl-impl] https://www.iwocl.org/resources/opencl-implementations/

.. [#wiki-ocl-osi] https://en.wikipedia.org/wiki/OpenCL#Open_source_implementations\

.. [#libclc] https://libclc.llvm.org

.. [#pocl-web] http://portablecl.org/

.. [#pocl-doc] http://portablecl.org/docs/html/

.. [#builtin-function-kernel] http://jonathan2251.github.io/lbt/clang.html#builtin-functions

.. [#open-cl-book-heter] https://dahlan.unimal.ac.id/files/ebooks2/2015%203rd%20Heterogeneous%20Computing%20with%20OpenCL%202.0.pdf

.. [#ocl-acceler-tf] https://mlsys.org/Conferences/doc/2018/84.pdf

.. [#ocl-agpvldl] https://www.researchgate.net/publication/350512301_Automatic_Graph_Partitioning_for_Very_Large-scale_Deep_Learning

.. [#sycl-aesstgplsmlw] https://easychair.org/publications/preprint/GjhX

.. [#ocl-acpcco] https://www.researchgate.net/publication/221084751_A_comprehensive_performance_comparison_of_CUDA_and_OpenCL

.. [#dlc-acs] https://arxiv.org/pdf/2002.03794.pdf

.. [#pocl-build] http://portablecl.org/docs/html/install.html#configure-build

.. [#pocl-dbg] http://portablecl.org/docs/html/debug.html

.. [#sycl] https://www.khronos.org/sycl

.. [#sycl-wiki] https://en.wikipedia.org/wiki/SYCL

.. [#dpcpp-book] https://itbook.store/books/9781484255735
