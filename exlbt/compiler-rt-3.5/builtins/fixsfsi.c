//===-- lib/fixsfsi.c - Single-precision -> integer conversion ----*- C -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements single-precision to integer conversion for the
// compiler-rt library.  No range checking is performed; the behavior of this
// conversion is undefined for out of range values in the C standard.
//
//===----------------------------------------------------------------------===//

#define SINGLE_PRECISION
#include "fp_lib.h"

ARM_EABI_FNALIAS(f2iz, fixsfsi)

int printf(const char *format, ...);
int sprintf(char *out, const char *format, ...);

COMPILER_RT_ABI int
__fixsfsi(fp_t a) {
    // Break a into sign, exponent, significand
    const rep_t aRep = toRep(a);
    const rep_t aAbs = aRep & absMask;
    const int sign = aRep & signBit ? -1 : 1;
    const int exponent = (aAbs >> significandBits) - exponentBias;
    const rep_t significand = (aAbs & significandMask) | implicitBit;
#if 0
    printf("significandBits=%x, ttt=%x, exponentBias=%x, signBit=%x\n", significandBits, ttt, exponentBias, (int)signBit);
    printf("REP_C(1)=%x, implicitBit=%x\n", REP_C(1), implicitBit);
    printf("sign=%x, exponent=%x, significand=%x\n", sign, exponent, significand);
    printf("aAbs=%x, significandMask=%x, implicitBit=%x, significandBits=%x, ttt=%x\n", aAbs, significandMask, implicitBit, significandBits, ttt);
#endif

    // If 0 < exponent < significandBits, right shift to get the result.
    if ((unsigned int)exponent < significandBits) {
        return sign * (significand >> (significandBits - exponent));
#if 0
        int aa = (int)(significandBits - exponent);
        int bb = (int)(significand >> aa);
        int cc = (int)(sign * bb);
        printf("sign=%08x, significand=%08x, aa=%08x, bb=%08x, cc=%08x\n", sign, (int)(significand & 0xffffffff), aa, bb, cc);
        return cc;
#endif
    }
    
    // If exponent is negative, the result is zero.
    else if (exponent < 0) {
        printf("2\n");
        return 0;
    }
    
    // If significandBits < exponent, left shift to get the result.  This shift
    // may end up being larger than the type width, which incurs undefined
    // behavior, but the conversion itself is undefined in that case, so
    // whatever the compiler decides to do is fine.
    else {
        printf("3\n");
        return sign * (significand << (exponent - significandBits));
    }
}
