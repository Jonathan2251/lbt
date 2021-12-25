//===-- absvsi2.c - Implement __absvsi2 -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements __absvsi2 for the compiler_rt library.
//
//===----------------------------------------------------------------------===//

// Same running result different for 64-bit and 32-bit cpu. Refer explanation in ch4_1_sub.cpp.

#include "../libsoftfloat/compiler-rt/builtins/int_lib.h"
//#include <stdio.h>

// Returns: absolute value

// Effects: aborts if abs(x) < 0

COMPILER_RT_ABI di_int __absvdi2(di_int a) {
  const int N = (int)(sizeof(di_int) * CHAR_BIT);
  if (a == ((di_int)1 << (N - 1)))
    compilerrt_abort();
  const di_int t = a >> (N - 1);
  return (a ^ t) - t;
}


int main() {
  di_int res = __absvdi2(0x7FFFFFFFFFFFFFFELL);
//  printf("res: %d\n", res);
}
