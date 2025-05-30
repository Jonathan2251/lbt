// RUN: %clang_builtins %s %librt -o %t && %run %t
// REQUIRES: librt_has_comparetf2

#include <stdio.h>

#if __LP64__ && __LDBL_MANT_DIG__ == 113

#include "fp_test.h"

int __getf2(long double a, long double b);

int test__getf2(long double a, long double b, enum EXPECTED_RESULT expected)
{
    int x = __getf2(a, b);
    int ret = compareResultCMP(x, expected);

    if (ret){
        printf("error in test__getf2(%.20Lf, %.20Lf) = %d, "
               "expected %s\n", a, b, x, expectedStr(expected));
    }
    return ret;
}

char assumption_1[sizeof(long double) * CHAR_BIT == 128] = {0};

#endif

int getf2_test()
{
#if __LP64__ && __LDBL_MANT_DIG__ == 113
    // NaN
    if (test__getf2(makeQNaN128(),
                    0x1.234567890abcdef1234567890abcp+3L,
                    LESS_0))
        return 1;
    // <
    // exp
    if (test__getf2(0x1.234567890abcdef1234567890abcp-3L,
                    0x1.234567890abcdef1234567890abcp+3L,
                    LESS_0))
        return 1;
    // mantissa
    if (test__getf2(0x1.234567890abcdef1234567890abcp+3L,
                    0x1.334567890abcdef1234567890abcp+3L,
                    LESS_0))
        return 1;
    // sign
    if (test__getf2(-0x1.234567890abcdef1234567890abcp+3L,
                    0x1.234567890abcdef1234567890abcp+3L,
                    LESS_0))
        return 1;
    // ==
    if (test__getf2(0x1.234567890abcdef1234567890abcp+3L,
                    0x1.234567890abcdef1234567890abcp+3L,
                    GREATER_EQUAL_0))
        return 1;
    // >
    // exp
    if (test__getf2(0x1.234567890abcdef1234567890abcp+3L,
                    0x1.234567890abcdef1234567890abcp-3L,
                    GREATER_EQUAL_0))
        return 1;
    // mantissa
    if (test__getf2(0x1.334567890abcdef1234567890abcp+3L,
                    0x1.234567890abcdef1234567890abcp+3L,
                    GREATER_EQUAL_0))
        return 1;
    // sign
    if (test__getf2(0x1.234567890abcdef1234567890abcp+3L,
                    -0x1.234567890abcdef1234567890abcp+3L,
                    GREATER_EQUAL_0))
        return 1;

#else
    //printf("skipped\n");
    return -1;

#endif
    return 0;
}
