//===----- lib/cpu0/fp_mode.c - Floaing-point mode utilities -----*- C -*-====//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "../builtins/fp_mode.h"

// __fe_xxx: Float Exception

FE_ROUND_MODE __fe_getround() {
//  return FE_TONEAREST;
  return FE_DOWNWARD;
}

int __fe_raise_inexact() {
  return 0;
}
