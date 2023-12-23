.. _sec-lib:

Library
========

.. contents::
   :local:
   :depth: 4

Floating point can be implemented both on software and hardware.

The 16-bit a*b can be calculated as follows,

- Precondition: a and b are normalized of IEEE half precision (16-bit) [#ieee754-half]_.
  Exponent bias: zero offset being 15: 15->0, 1-> -14, 30 -> 15. And 31 -> NaN.

- Transformation for a*b:

  - 1. {sign-bit(a) xor sign-bit(b)} {exponent-bits(a)+exponent-bits(b)-15} {significand-bits(a)*significand-bits(b) >> 10}
  - 2. Normalize:

- ex.

  - a = 0.01 (binary) = {0 01110 1000000000}; b = 0.11 (binary) = {0 10000 1100000000}

  - 1. a*b = {0 xor 0} {01110+10000-01111=01111} {1000000000*1100000000 >> 10 = 0110000000}
  - 2. Normalize: {0 01111 0110000000} -> {0 01110 1100000000}

Since Cpu0 has not hardware float point instructions, it needs software float 
point library to finish the floating point operation. 
LLVM compiler-rt project has software floating point library implementation 
:numref:`swf-lib` , so I choose it as the implementation. 

Since compiler-rt uses unix/linux rootfs structure, we fill the gap by add few
empty include-files in exlbt/include.

.. _swf-lib:
.. graphviz:: ../Fig/lib/lib.gv
  :caption: compiler-rt\/lib\/builtins' software float library

The dependences for compiler-rt on libm as :numref:`compiler-rrt-dep`.
  
.. _compiler-rrt-dep-short:
.. graphviz:: ../Fig/lib/compiler-rt-dep-short.gv
  :caption: Dependences for compiler-rt on libm
  

.. table:: lldb dependences

  ==============  ========================== 
  functions       depend on
  ==============  ==========================
  scanf           newlib/libc
  printf          sanitizer_printf.c of compiler-rt
  ==============  ==========================

.. table:: sanitizer_printf.c of compiler-rt dependences

  ====================  ========================== 
  functions             depend on
  ====================  ==========================
  sanitizter_printf.c   builtins of compiler-rt
  ====================  ==========================


C Library (Newlib)
------------------

Since complex type of compiler-rt depends on libm, I porting NewLib in this 
section.

Newlib is a C library for bare metal platform.
Two libraries in newlib are libc and libm. Libc is for functions of
IO, file and string supported while libm is for mathematical functions.
Web of newlib is here [#newlib]_ and newlib/libm here [#newlib-libm]_ .
Since the next section compiler-rt/builtins depends on libm, please run
the following bash script to install and build newlib for Cpu0.

.. rubric:: lbt/exlbt/newlib-cpu0.sh
.. literalinclude:: ../exlbt/newlib-cpu0.sh

.. note:: **In order to add Cpu0 backend to NewLib, the following changes in lbt/exlbt/newlib.patch** 

  - lbt/exlbt/newlib-cygwin/newlib/libc/machine/cpu0/setjmp.S is added; 

  - newlib-cygwin/config.sub, newlib-cygwin/newlib/configure.host, 
    newlib-cygwin/newlib/libc/include/machine/ieeefp.h, 
    newlib-cygwin/newlib/libc/include/sys/unistd.h and
    newlib-cygwin/newlib/libc/machine/configure are modified for adding cpu0.

.. rubric:: lbt/exlbt/newlib.patch
.. literalinclude:: ../exlbt/newlib.patch

.. rubric:: lbt/exlbt/newlib-cygwin/newlib/libc/machine/cpu0/setjmp.S
.. literalinclude:: ../exlbt/newlib-cygwin/newlib/libc/machine/cpu0/setjmp.S

.. code-block:: console

  cschen@cschendeiMac exlbt % bash newlib-cpu0.sh

The libm.a depends on variable errno of libc defined in sys/errno.h.

- libgloss is BSP [#libgloss-bsp]_

Compiler-rt's builtins
----------------------

Compiler-rt is a project for runtime libraries implentation [#compiler-rt]_ .
Compiler-rt/lib/builtins provides functions for basic operations such as +, -, 
\*, /, ... on type of float or double and for conversion between float and 
integer, or other type of more than 32-bit, such as `long long`. 
The compiler-rt/lib/builtins/README.txt 
[#builtins-README]_ includes the dependent functions that the whole builtins 
called.
The dependent functions is a small part of libm listed in 
compier-rt/lib/builtins/int_math.h [#builtins-int_math]_ .

.. rubric:: ~git/newlib-cygwin/build-cpu032I-eb/Makefile
.. code-block:: Makefile

  MATHDIR = math

  # The newlib hardware floating-point routines have been disabled due to
  # inaccuracy.  If you wish to work on them, you will need to edit the
  # configure.in file to re-enable the configuration option.  By default,
  # the NEWLIB_HW_FP variable will always be false.
  #MATHDIR = mathfp 

As above Makefile, newlib uses libm/math.
The dependences for builtin functions of compiler-rt on libm as 
:numref:`compiler-rrt-dep`.

.. _compiler-rrt-dep:
.. graphviz:: ../Fig/lib/compiler-rt-dep.gv
  :caption: Dependences for builtin functions of compiler-rt on libm
  
In this section, I get test cases for verification of SW Float Point from
compiler-rt/test/builtins/Unit to compiler-rt-test/builtins/Unit/.

Since lbt/exlbt/input/printf-stdarg.c does not support %lld (long long 
integeter, 64-bit) and test cases in compiler-rt/test/builtins/Unit needs it to
verify the result of test cases for SW Float Point, I port sanitizer_printf.cpp 
and sanitizer_internal_defs.h of lbt/exlbt/input from sanitizer_printf.cpp and 
sanitizer_internal_defs.h of compiler-rt/lib/sanitizer_common.

.. table:: compiler-rt builtins dependences on newlib/libm (open source libc 
           for bare metal) 

  ==============  =============================  ==============
  function        file                           directory of libm 
  ==============  =============================  ==============
  abort           lbt/exlbt/compiler-rt/
                  cpu0/abort.c
  isinf           s_isinf.c                      newlib-cygwin/newlib/libm/common
  isnan           s_isnan.c                        "
  fabsl           fabsl.c                          "
  fmax            s_fmax.c                         "
  fmaxf           sf_fmax.c                        "
  fmaxl           fmaxl.c                          "
  log             log.c                            "
  logf            sf_log.c                         "
  logl            logl.c                           "
  scalbn          s_scalbn.c                       "
  scalbnf         sf_scalbn.c                      "
  scalbnl         scalblnl.c                       "
  copysign        s_copysign.c                     "
  copysignf       sf_copysign.c                    "
  copysignl       copysignl.c                      "
  fabsl           fabsl.c                          "
  --------------  -----------------------------  --------------
  fabs            s_fabs.c                       newlib-cygwin/newlib/libm/math
  fabsf           sf_fabs.c                        "
  ==============  =============================  ==============

- Libm has no dependence to any other library.

- Only type of complex in compiler-rt/lib/builtin need above, others (float and 
  double) depend on __builtin_clz(), __builtin_clo() and abort() only. I has 
  ported in lbt/exlbt/compiler-rt/cpu0/abort.c.

- All test cases in compiler-rt/test/builtins/Unit depend on 
  printf(%lld or %llX, ...), I ported from 
  compiler-rt/lib/sanitizer_common/sanitizer_printf.cpp to 
  lbt/exlbt/input/sanitizer_printf.cpp.

- These dependent functions of complex type has bee ported from newlib/libm.

- Except builtins, the other three, sanitizer runtimes, profile and BlocksRuntime, 
  in compiler-rt are not needed for my embedded Cpu0.

The libgcc's Integer plus Soft float library  [#lib-gcc]_ [#int-lib]_ 
[#sw-float-lib]_ are equal to functions of compiler-rt's builtins.

Though the 'rt' means RunTime libaraies, in builtins library, 
most of these functions 
written in target-independent C form and can be compiled and static-linked
into target. When you compile the following c code, llc will generate 
**jsub __addsf3** to call compiler-rt float function for Cpu0.
This is because Cpu0 hasn't hardware float-instructions, so Cpu0 backend 
doesn't handle the DAG of __addsf3. The end result, llvm treats the DAG of 
__addsf3 as a function call for float-add instruction.

.. rubric:: lbt/exlbt/input/ch_call_compilerrt_func.c
.. literalinclude:: ../exlbt/input/ch_call_compilerrt_func.c

.. code-block:: console

  chungshu@ChungShudeMacBook-Air input % clang -target mips-unknown-linux-gnu -S 
  ch_call_compilerrt_func.c -emit-llvm
  chungshu@ChungShudeMacBook-Air input % cat ch_call_compilerrt_func.ll
    ...
    %4 = load float, float* %1, align 4
    %5 = load float, float* %2, align 4
    %6 = fadd float %4, %5

  chungshu@ChungShudeMacBook-Air input % ~/llvm/test/build/bin/llc -march=cpu0 
  -mcpu=cpu032II -relocation-model=static -filetype=asm ch_call_compilerrt_func.ll -o -
	...
	ld	$4, 20($fp)
	ld	$5, 16($fp)
	jsub	__addsf3


For some brar-metal or embedded application, the C code doesn't need the
file and high-level IO in libc.
Libm provides a lots of functions to support software floating point beyond
basic operations [#math]_ .
Libc provides file, high-level IO functions and basic float functions [#clib]_ .

Cpu0 hires Compiler-rt/lib/builtins and 
compiler-rt/lib/sanitizer_common/sanitizer_printf.cpp at this point.
The compiler-rt/lib/builtins is a 
target-independent C form of software float library implementation. Cpu0 
implements compiler-rt-12.x/cpu0/abort.c only at this point for supporting 
this feature.

.. note:: **Why these libm functions called builtins in compiler-rt/lib/builtins?**

  Though these compiler-rt builtins functions are written in C. The CPU can
  provide float type instructions or high level instructions to compile these
  libm function calls into specific HW instructions to speed up.

In order to speed up these libm functions, many CPU provide float instructions
for them. Of course, for the implemenation, clang compiles these float type's
operation in C into llvm ir, then Mips backends compiles them into their 
HW instructions. For example:

- float a, b, c; a=b*c; -> (clang) -> %add = fmul float %0, %1 [#llvm-fmul]_

Mips backend compiles `fmul` into HW instructions as follows,

- %add = fmul float %0, %1 -> (llvm-mips) -> mul.s [#llvm-fmul]_ [#mips-fmadd1.ll]_

Cpu0 backend compiles `fmul` into libm function call fmul as follows,

- %add = fmul float %0, %1 -> (llvm-mips) -> jsub fmul [#llvm-fmul]_

For high level of math functions, clang compiles these float type's
operation in C into llvm intrinsic functions, then the llvm backends of these
CPU compile them into their HW instructions. For example, clang compiles pow()
into @llvm.pow.f32 as follows,

- %pow = call float @llvm.pow.f32(float %x, float %y) [#clang-pow]_

AMDGPU compiles @llvm.pow.f32 into a few instructions as follows:

- %pow = call float @llvm.pow.f32(float %x, float %y) (llvm-AMDGPU) -> ... + v_exp_f32_e32 v0, v0 + ... [#clang-pow]_

Mips compiles @llvm.pow.f32 into a few instructions as follows:

- %pow = call float @llvm.pow.f32(float %x, float %y) (llvm-AMDGPU) -> jal powf [#clang-pow]_


Clang treats these libm functions as builtin and compiles them into llvm ir or 
intrinsic, then different backends can choose to compile them into specific 
instructions or call builtin functions in libm. The following is Clang's 
comment [#clang-builtin-comment]_.

.. code-block:: c++

  RValue CodeGenFunction::EmitBuiltinExpr(...)
    ...
    // There are LLVM math intrinsics/instructions corresponding to math library 
    // functions except the LLVM op will never set errno while the math library
    // might. Also, math builtins have the same semantics as their math library
    // twins. Thus, we can transform math library and builtin calls to their
    // LLVM counterparts if the call is marked 'const' (known to never set errno).


Verification
~~~~~~~~~~~~

The following sanitizer_printf.cpp extended from compiler-rt can support 
printf("%lld"). It's implementation calling some floating lib functions
in compiler-rt/lib/builtins.

.. rubric:: exlbt/include/math.h
.. literalinclude:: ../exlbt/include/math.h

.. rubric:: exlbt/include/stdio.h
.. literalinclude:: ../exlbt/include/stdio.h

.. rubric:: exlbt/include/stdlib.h
.. literalinclude:: ../exlbt/include/stdlib.h

.. rubric:: exlbt/include/string.h
.. literalinclude:: ../exlbt/include/string.h

.. rubric:: exlbt/compiler-rt/cpu0/abort.c
.. literalinclude:: ../exlbt/compiler-rt/cpu0/abort.c

.. rubric:: exlbt/input/sanitizer_internal_defs.h
.. literalinclude:: ../exlbt/input/sanitizer_internal_defs.h

.. rubric:: exlbt/input/sanitizer_printf.cpp
.. literalinclude:: ../exlbt/input/sanitizer_printf.cpp


Above two sanitizer_*.* files are ported from compiler-rt and I add code to
support left-justify for number-printf and right-justify for string-printf.
The following ch_float.cpp test the float lib.

.. rubric:: lbt/exlbt/compiler-rt-12.x/builtins/Makefile
.. literalinclude:: ../exlbt/compiler-rt-12.x/builtins/Makefile

.. rubric:: exlbt/input/ch_float.cpp
.. literalinclude:: ../exlbt/input/ch_float.cpp
    :start-after: /// start

.. rubric:: exlbt/input/Makefile.float
.. literalinclude:: ../exlbt/input/Makefile.float


.. code-block:: console

  chungshu@ChungShudeMacBook-Air input % bash make.sh cpu032II eb Makefile.float
  ...
  endian =  BigEndian
  ISR address:00020614
  0   /* 0: big endian, 1: little endian */

  chungshu@ChungShudeMacBook-Air verilog % iverilog -o cpu0IIs cpu0IIs.v 
  chungshu@ChungShudeMacBook-Air verilog % ./cpu0IIs
  ...

  a: 100000007FFFFFFF, 100000007fffffff, 1152921506754330623
  b: 10000000, 268435456
  b: 10000000, 268435456
  Hello world!
  printf test
  <null> is null pointer
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
  test_longlong_shift1() = 289
  test_longlong_shift2() = 22
  test_shift_left<long long>(0x12, 4LL) = 288
  test_shift_right<long long>(0x001666660000000a, 48LL) = 22
  test_shift_right<unsigned long long>(0x001666660000000a, 48LLu) = 22
  (int)test_add<float, float, float>(-2.2, 3.3) = 1
  (int)test_mul<float, float, float>(-2.2, 3.3) = -7
  (int)test_div<float, float, float>(-1.8, 0.5) = -3
  (int)test_add<double, double, float>(-2.2, 3.3) = 1
  (int)test_add<double, float, double>(-2.2, 3.3) = 1
  (int)test_add<float, float, double>(-2.2, 3.3) = 1
  (int)test_mul<double, float, double>(-2.2, 3.3) = -7
  (int)test_mul<float, float, double>(-2.2, 3.3) = -7
  (int)test_div<double, double, double>(-1.8, 0.5) = -3
  test_div<long long, long long, long long>(-10LL, 4LL) = -2
  ...             
  RET to PC < 0, finished!


The exlbt/input/compiler-rt-test/builtins/Unit copied from 
compiler-rt/test/builtins/Unit as follows,

.. rubric:: exlbt/input/ch_builtins.cpp
.. literalinclude:: ../exlbt/input/ch_builtins.cpp

.. rubric:: exlbt/input/Makefile.builtins
.. literalinclude:: ../exlbt/input/Makefile.builtins

Run as follows,

.. code-block:: console

  chungshu@ChungShudeMacBook-Air input % bash make.sh cpu032II eb Makefile.builtins
  ...
  chungshu@ChungShudeMacBook-Air verilog % ./cpu0IIs
  ...
  absvdi2_test(): PASS!
  absvsi2_test(): PASS!
  absvti2_test(): SKIPPED!
  adddf3vfp_test(): SKIPPED!
  addsf3vfp_test(): SKIPPED!
  addvdi3_test(): PASS!
  addvsi3_test(): PASS!
  addvti3_test(): SKIPPED!
  ashldi3_test(): PASS!
  ashlti3_test(): SKIPPED!
  ashrdi3_test(): PASS!
  ashrti3_test(): SKIPPED!
  bswapdi2_test(): PASS!
  bswapsi2_test(): PASS!
  clzdi2_test(): PASS!
  clzsi2_test(): PASS!
  clzti2_test(): SKIPPED!
  cmpdi2_test(): PASS!
  cmpti2_test(): SKIPPED!
  comparedf2_test(): PASS!
  comparesf2_test(): PASS!
  cpu_model_test(): SKIPPED!
  ctzdi2_test(): PASS!
  ctzsi2_test(): PASS!
  ctzti2_test(): SKIPPED!
  divdc3_test(): PASS!
  divdf3_test(): PASS!
  divdf3vfp_test(): SKIPPED!
  divdi3_test(): PASS!
  divmodsi4_test(): PASS!
  divmodti4_test(): SKIPPED!
  divsf3_test(): PASS!
  divsf3vfp_test(): SKIPPED!
  divsi3_test(): PASS!
  divtc3_test(): PASS!
  divtf3_test(): SKIPPED!
  divti3_test(): SKIPPED!
  divxc3_test(): PASS!
  eqdf2vfp_test(): SKIPPED!
  eqsf2vfp_test(): SKIPPED!
  eqtf2_test(): SKIPPED!
  extenddftf2_test(): SKIPPED!
  extendhfsf2_test(): PASS!
  extendhftf2_test(): SKIPPED!
  extendsfdf2vfp_test(): SKIPPED!
  extendsftf2_test(): SKIPPED!
  gedf2vfp_test(): SKIPPED!
  gesf2vfp_test(): SKIPPED!
  getf2_test(): SKIPPED!
  gtdf2vfp_test(): SKIPPED!
  gtsf2vfp_test(): SKIPPED!
  gttf2_test(): SKIPPED!
  ledf2vfp_test(): SKIPPED!
  lesf2vfp_test(): SKIPPED!
  letf2_test(): SKIPPED!
  lshrdi3_test(): PASS!
  lshrti3_test(): SKIPPED!
  ltdf2vfp_test(): SKIPPED!
  ltsf2vfp_test(): SKIPPED!
  lttf2_test(): SKIPPED!
  moddi3_test(): PASS!
  modsi3_test(): PASS!
  modti3_test(): SKIPPED!
  muldc3_test(): PASS!
  muldf3vfp_test(): SKIPPED!
  muldi3_test(): PASS!
  mulodi4_test(): PASS!
  mulosi4_test(): PASS!
  muloti4_test(): SKIPPED!
  mulsc3_test(): PASS!
  mulsf3vfp_test(): SKIPPED!
  multc3_test(): SKIPPED!
  multf3_test(): SKIPPED!
  multi3_test(): SKIPPED!
  mulvdi3_test(): PASS!
  mulvsi3_test(): PASS!
  mulvti3_test(): SKIPPED!
  mulxc3_test(): PASS!
  nedf2vfp_test(): SKIPPED!
  negdf2vfp_test(): SKIPPED!
  negdi2_test(): PASS!
  negsf2vfp_test(): SKIPPED!
  negti2_test(): SKIPPED!
  negvdi2_test(): PASS!
  negvsi2_test(): PASS!
  negvti2_test(): SKIPPED!
  nesf2vfp_test(): SKIPPED!
  netf2_test(): SKIPPED!
  subdf3vfp_test(): SKIPPED!
  subsf3vfp_test(): SKIPPED!
  subtf3_test(): SKIPPED!
  subvdi3_test(): PASS!
  subvsi3_test(): PASS!
  subvti3_test(): SKIPPED!
  trampoline_setup_test(): SKIPPED!
  truncdfhf2_test(): PASS!
  truncdfsf2_test(): PASS!
  truncdfsf2vfp_test(): SKIPPED!
  truncsfhf2_test(): PASS!
  trunctfdf2_test(): SKIPPED!
  trunctfhf2_test(): SKIPPED!
  trunctfsf2_test(): SKIPPED!
  ucmpdi2_test(): PASS!
  ucmpti2_test(): SKIPPED!
  udivdi3_test(): PASS!
  udivmoddi4_test(): PASS!
  udivmodsi4_test(): PASS!
  udivmodti4_test(): SKIPPED!
  udivsi3_test(): PASS!
  udivti3_test(): SKIPPED!
  umoddi3_test(): PASS!
  umodsi3_test(): PASS!
  umodti3_test(): SKIPPED!
  unorddf2vfp_test(): SKIPPED!
  unordsf2vfp_test(): SKIPPED!
  unordtf2_test(): SKIPPED!
  ...          
  RET to PC < 0, finished!


.. [#ieee754-half] https://en.wikipedia.org/wiki/Half-precision_floating-point_format

.. [#newlib] https://sourceware.org/newlib/

.. [#newlib-libm] https://sourceware.org/newlib/libm.html

.. [#libgloss-bsp] https://www.embecosm.com/appnotes/ean9/html/ch03s01.html

.. [#compiler-rt] http://compiler-rt.llvm.org/

.. [#builtins-README] https://github.com/llvm-mirror/compiler-rt/blob/master/lib/builtins/README.txt

.. [#builtins-int_math] https://github.com/microsoft/compiler-rt/blob/master/lib/builtins/int_math.h

.. [#lib-gcc] https://gcc.gnu.org/onlinedocs/gccint/Libgcc.html

.. [#int-lib] https://gcc.gnu.org/onlinedocs/gccint/Integer-library-routines.html#Integer-library-routines

.. [#sw-float-lib] https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html#Soft-float-library-routines

.. [#math] https://www.programiz.com/c-programming/library-function/math.h

.. [#clib] https://www.cplusplus.com/reference/clibrary

.. [#mips-fmadd1.ll] https://github.com/llvm/llvm-project/blob/main/llvm/test/CodeGen/Mips/fmadd1.ll

.. [#llvm-fmul] Reference https://github.com/Jonathan2251/lbd/tree/master/lbdex/input/ch7_1_fmul.c

.. [#clang-pow] Reference https://github.com/Jonathan2251/lbt/tree/master/exlbt/input/test_pow.c

.. [#clang-builtin-comment] https://github.com/llvm/llvm-project/blob/main/clang/lib/CodeGen/CGBuiltin.cpp
