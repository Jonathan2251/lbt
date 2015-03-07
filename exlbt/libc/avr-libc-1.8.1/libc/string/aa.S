; ModuleID = 'aa.S'
target datalayout = "E-m:m-p:32:32-i8:8:32-i16:16:32-i64:64-n32-S64"
target triple = "mips-unknown-linux-gnu"

;module 	asm(".global	_U(memset)");
;module 	asm(".type	_U(memset), @function");
module asm("_U(memset):");
module         asm("ret $lr");
module asm(".L_memset_end:");
module 	asm(".size	_U(memset), .L_memset_end - _U(memset)");
