// RUN: %clang_builtins %s %librt -o %t && %run %t
// REQUIRES: librt_has_clzdi2

#include "int_lib.h"
#include <stdio.h>

// Returns: the number of leading 0-bits

// Precondition: a != 0

//COMPILER_RT_ABI int __clzdi2(di_int a);

COMPILER_RT_ABI int __clzdi2(di_int a) {
  dwords x;
  x.all = a;
  const si_int f = -(x.s.high == 0);
  printf("x.s.high = %d, x.s.low = %d, f = %d, ~f = %d\n", x.s.high, x.s.low, f, ~f);
  printf("(x.s.high & ~f) = %d\n", (x.s.high & ~f));
  printf("(x.s.low & f) = %d\n", (x.s.low & f));
  printf("clzsi((x.s.high & ~f) | (x.s.low & f)) = %d\n", clzsi((x.s.high & ~f) | (x.s.low & f)));
  return clzsi((x.s.high & ~f) | (x.s.low & f)) +      // = (1280|0)+0 = 1280
         (f & ((si_int)(sizeof(si_int) * CHAR_BIT)));
}


int test__clzdi2(di_int a, int expected)
{
    int x = __clzdi2(a);
    printf("__clzdi2(a) = %d, expected %d\n", x, expected);
    if (x != expected)
        //printf("error in __clzdi2(0x%llX) = %d, expected %d\n", a, x, expected);
        printf("error in __clzdi2(%d - %d) = %d, expected %d\n", (int)(a>>32), (int)a, x, expected);
    return x != expected;
}

static char assumption_1[sizeof(di_int) == 2*sizeof(si_int)] = {0};

int clzdi2_test()
{
    const int N = (int)(sizeof(di_int) * CHAR_BIT);
//    if (test__clzdi2(0x00000000, N))  // undefined
//        return 1;
    if (test__clzdi2(0x00000001, N-1))
        return 1;
    if (test__clzdi2(0x00000002, N-2))
        return 1;
    if (test__clzdi2(0x00000003, N-2))
        return 1;
    if (test__clzdi2(0x00000004, N-3))
        return 1;
    if (test__clzdi2(0x00000005, N-3))
        return 1;
    if (test__clzdi2(0x0000000A, N-4))
        return 1;
    if (test__clzdi2(0x1000000A, N/2+3))
        return 1;
    if (test__clzdi2(0x2000000A, N/2+2))
        return 1;
    if (test__clzdi2(0x6000000A, N/2+1))
        return 1;
    if (test__clzdi2(0x8000000AuLL, N/2))
        return 1;
    if (test__clzdi2(0x000005008000000AuLL, 21))
        return 1;
    if (test__clzdi2(0x020005008000000AuLL, 6))
        return 1;
    if (test__clzdi2(0x720005008000000AuLL, 1))
        return 1;
    if (test__clzdi2(0x820005008000000AuLL, 0))
        return 1;

   return 0;
}