/* ===-- absvsi2.c - Implement __absvsi2 -----------------------------------===
 *
 *                     The LLVM Compiler Infrastructure
 *
 * This file is dual licensed under the MIT and the University of Illinois Open
 * Source Licenses. See LICENSE.TXT for details.
 *
 * ===----------------------------------------------------------------------===
 *
 * This file implements __absvsi2 for the compiler_rt library.
 *
 * ===----------------------------------------------------------------------===
 */

#include "int_lib.h"

/* Returns: absolute value */

/* Effects: aborts if abs(x) < 0 */

extern int printf(const char *format, ...);
COMPILER_RT_ABI si_int
__absvsi2(si_int a)
{
#if 0
    const int N = (int)(sizeof(si_int) * CHAR_BIT);
    if (a == (1 << (N-1))) {
        compilerrt_abort();
    }
    const si_int t = a >> (N - 1);
    return (a ^ t) - t;
#else
  // Fix for Cpu0
    si_int result;
    const int N = (int)(sizeof(si_int) * CHAR_BIT);
    if (a == (1 << (N-1))) {
    // In 2's compliment, positive only reach (power(2, N-1) - 1)
        compilerrt_abort();
    }
    const si_int t = (1 << (N-1));
    if ((a & t) == t)
      // negative, t = 0x8000...0
      result = 0 - a;
    else
      result = a;
    return result;
#endif
}
