.. _sec-opt:

Optimization
=============

.. contents::
   :local:
   :depth: 4

This chapter introduce llvm optimization. 


LLVM IR optimization
----------------------

The llvm-link provide optimizaton in IR level which can apply in different 
programs developed by more than one language. Of course, it can apply in 
the same language which support seperate compile.

.. _llvm-link-f-flow: 
.. figure:: ../Fig/opt/1.png
  :scale: 100 %
  :align: center

  llvm-link flow

Clang provide optimization options to do optimation from high level language 
to IR. 
But since many languages like C/C++ support separate compilation, it meaning 
there is no chance to do inter-procedure optimization if the functions come from  
different source files.
To solve this problem, llvm provide **llvm-link** to link all \*.bc into a single
IR file, and through **opt** to finish the inter-procedure optimation [#]_.
Beyond the DAG local optimization mentioned in Chapter 2, there are global 
optimization based on inter-procedure analysis [#dragonbooks-9]_.
The following steps and examples show this optimization solution in llvm.

.. rubric:: exlbt/input/optimizen/1.cpp
.. literalinclude:: ../exlbt/input/optimize/1.cpp
    :start-after: /// start

.. rubric:: exlbt/input/optimize/2.cpp
.. literalinclude:: ../exlbt/input/optimize/2.cpp
    :start-after: /// start


.. code-block:: console

  JonathantekiiMac:input Jonathan$ clang -O3 -target mips-unknown-linux-gnu 
  -c 1.cpp -emit-llvm -o 1.bc
  JonathantekiiMac:input Jonathan$ clang -O3 -target mips-unknown-linux-gnu 
  -c 2.cpp -emit-llvm -o 2.bc
  JonathantekiiMac:input Jonathan$ llvm-link -o=a.bc 1.bc 2.bc
  JonathantekiiMac:input Jonathan$ opt -O3 -o=a1.bc a.bc
  JonathantekiiMac:input Jonathan$ llvm-dis a.bc -o -
  ...
  ; Function Attrs: nounwind readonly
  define i32 @_Z6calleePKi(i32* nocapture readonly %a) #0 {
    %1 = load i32* %a, align 4, !tbaa !1
    %2 = add nsw i32 %1, 1
    ret i32 %2
  }

  define i32 @_Z6callerv() #1 {
    %T = alloca i32, align 4
    store i32 4, i32* %T, align 4, !tbaa !1
    %1 = call i32 @_Z6calleePKi(i32* %T)
    ret i32 %1
  }
  ...

  JonathantekiiMac:input Jonathan$ llvm-dis a1.bc -o -
  ...
  ; Function Attrs: nounwind readonly
  define i32 @_Z6calleePKi(i32* nocapture readonly %a) #0 {
    %1 = load i32* %a, align 4, !tbaa !1
    %2 = add nsw i32 %1, 1
    ret i32 %2
  }

  ; Function Attrs: nounwind readnone
  define i32 @_Z6callerv() #1 {
    ret i32 5
  }
  ...

From the result as above, the **opt** output has lesser number of IR 
instructions. 
Of course, the backend code will be more effective as follows,

.. code-block:: console

  JonathantekiiMac:input Jonathan$ /Users/Jonathan/llvm/test/cmake_debug_build/
  bin/Debug/llc -march=cpu0 -relocation-model=pic -filetype=asm a.bc -o -
	  .section .mdebug.abi32
	  .previous
	  .file	"a.bc"
	  .text
	  .globl	_Z6calleePKi
	  .align	2
	  .type	_Z6calleePKi,@function
	  .ent	_Z6calleePKi            # @_Z6calleePKi
  _Z6calleePKi:
	  .frame	$sp,0,$lr
	  .mask 	0x00000000,0
	  .set	noreorder
	  .set	nomacro
  # BB#0:
	  ld	$2, 0($sp)
	  ld	$2, 0($2)
	  addiu	$2, $2, 1
	  ret	$lr
	  .set	macro
	  .set	reorder
	  .end	_Z6calleePKi
  $tmp0:
	  .size	_Z6calleePKi, ($tmp0)-_Z6calleePKi

	  .globl	_Z6callerv
	  .align	2
	  .type	_Z6callerv,@function
	  .ent	_Z6callerv              # @_Z6callerv
  _Z6callerv:
	  .cfi_startproc
	  .frame	$sp,32,$lr
	  .mask 	0x00004000,-4
	  .set	noreorder
	  .cpload	$t9
	  .set	nomacro
  # BB#0:
	  addiu	$sp, $sp, -32
  $tmp3:
	  .cfi_def_cfa_offset 32
	  st	$lr, 28($sp)            # 4-byte Folded Spill
  $tmp4:
	  .cfi_offset 14, -4
	  .cprestore	8
	  addiu	$2, $zero, 4
	  st	$2, 24($sp)
	  addiu	$2, $sp, 24
	  st	$2, 0($sp)
	  ld	$t9, %call16(_Z6calleePKi)($gp)
	  jalr	$t9
	  ld	$gp, 8($sp)
	  ld	$lr, 28($sp)            # 4-byte Folded Reload
	  addiu	$sp, $sp, 32
	  ret	$lr
	  .set	macro
	  .set	reorder
	  .end	_Z6callerv
  $tmp5:
	  .size	_Z6callerv, ($tmp5)-_Z6callerv
	  .cfi_endproc

  JonathantekiiMac:input Jonathan$ /Users/Jonathan/llvm/test/cmake_debug_build/
  bin/Debug/llc -march=cpu0 -relocation-model=pic -filetype=asm a1.bc -o -
	  .section .mdebug.abi32
	  .previous
	  .file	"a1.bc"
	  .text
	  .globl	_Z6calleePKi
	  .align	2
	  .type	_Z6calleePKi,@function
	  .ent	_Z6calleePKi            # @_Z6calleePKi
  _Z6calleePKi:
	  .frame	$sp,0,$lr
	  .mask 	0x00000000,0
	  .set	noreorder
	  .set	nomacro
  # BB#0:
	  ld	$2, 0($sp)
	  ld	$2, 0($2)
	  addiu	$2, $2, 1
	  ret	$lr
	  .set	macro
	  .set	reorder
	  .end	_Z6calleePKi
  $tmp0:
	  .size	_Z6calleePKi, ($tmp0)-_Z6calleePKi

	  .globl	_Z6callerv
	  .align	2
	  .type	_Z6callerv,@function
	  .ent	_Z6callerv              # @_Z6callerv
  _Z6callerv:
	  .frame	$sp,0,$lr
	  .mask 	0x00000000,0
	  .set	noreorder
	  .set	nomacro
  # BB#0:
	  addiu	$2, $zero, 5
	  ret	$lr
	  .set	macro
	  .set	reorder
	  .end	_Z6callerv
  $tmp1:
	  .size	_Z6callerv, ($tmp1)-_Z6callerv

Though llvm-link provide optimization in IR level to support seperate compile,
it come with the cost in compile time. As you can imagine, any one statement 
change will change the output IR of llvm-link. And the obj binary code have to 
re-compile. Compare to the seperate compile for each \*.c file, it only need to
re-compile the corresponding \*.o file only.


Project
--------

LLVM-VPO
~~~~~~~~~

Friend Gang-Ryung Uh replace LLC compiler by llvm on Very Portable Optimizer 
(VPO) compiler toolchain. VPO performs optimizations on a single intermediate 
representation called Register Transfer Lists (RTLs). In other word, the system 
generate RTLs from llvm IR and it do further optimization on RTLs.

The LLVM-VPO is illustrated at his home page. Click **"6. LLVM-VPO Compiler 
Development - 2012 Google Faculty Research Award"** at this home page 
[#Gang-Ryung]_ will get the information.


.. [#] http://www.cs.cmu.edu/afs/cs/academic/class/15745-s12/public/lectures/L3-LLVM-Part1.pdf

.. [#dragonbooks-9] Refer chapter 9 of book Compilers: Principles, 
    Techniques, and Tools (2nd Edition) 

.. [#Gang-Ryung] http://cs.boisestate.edu/~uh/

