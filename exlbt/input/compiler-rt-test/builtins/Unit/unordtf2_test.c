// RUN: %clang_builtins %s %librt -o %t && %run %t
// REQUIRES: librt_has_comparetf2

#include <stdio.h>

#if __LP64__ && __LDBL_MANT_DIG__ == 113

#include "fp_test.h"

int __unordtf2(long double a, long double b);

int test__unordtf2(long double a, long double b, enum EXPECTED_RESULT expected)
{
    int x = __unordtf2(a, b);
    int ret = compareResultCMP(x, expected);

    if (ret){
        printf("error in test__unordtf2(%.20Lf, %.20Lf) = %d, "
               "expected %s\n", a, b, x, expectedStr(expected));
    }
    return ret;
}

static char assumption_1[sizeof(long double) * CHAR_BIT == 128] = {0};

#endif

int unordtf2_test()
{
#if __LP64__ && __LDBL_MANT_DIG__ == 113
    // NaN
    if (test__unordtf2(makeQNaN128(),
                       0x1.234567890abcdef1234567890abcp+3L,
                       NEQUAL_0))
        return 1;
    // other
    if (test__unordtf2(0x1.234567890abcdef1234567890abcp+3L,
                       0x1.334567890abcdef1234567890abcp+3L,
                       EQUAL_0))
        return 1;
    if (test__unordtf2(0x1.234567890abcdef1234567890abcp+3L,
                       0x1.234567890abcdef1234567890abcp+3L,
                       EQUAL_0))
        return 1;
    if (test__unordtf2(0x1.234567890abcdef1234567890abcp+3L,
                       0x1.234567890abcdef1234567890abcp-3L,
                       EQUAL_0))
        return 1;

#else
    //printf("skipped\n");
    return -1;

#endif
    return 0;
}
