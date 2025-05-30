// RUN: %clang_builtins %s %librt -o %t && %run %t
// REQUIRES: librt_has_clzsi2

#include "int_lib.h"
#include <stdio.h>

// Returns: the number of leading 0-bits

// Precondition: a != 0

COMPILER_RT_ABI int __clzsi2(si_int a);

int test__clzsi2(si_int a, int expected)
{
    int x = __clzsi2(a);
    if (x != expected)
        printf("error in __clzsi2(0x%X) = %d, expected %d\n", a, x, expected);
    return x != expected;
}

static char assumption_1[sizeof(di_int) == 2*sizeof(si_int)] = {0};
static char assumption_2[sizeof(si_int)*CHAR_BIT == 32] = {0};

int clzsi2_test()
{
//    if (test__clzsi2(0x00000000, 32))  // undefined
//        return 1;
    if (test__clzsi2(0x00800000, 8))
        return 1;
    if (test__clzsi2(0x01000000, 7))
        return 1;
    if (test__clzsi2(0x02000000, 6))
        return 1;
    if (test__clzsi2(0x03000000, 6))
        return 1;
    if (test__clzsi2(0x04000000, 5))
        return 1;
    if (test__clzsi2(0x05000000, 5))
        return 1;
    if (test__clzsi2(0x06000000, 5))
        return 1;
    if (test__clzsi2(0x07000000, 5))
        return 1;
    if (test__clzsi2(0x08000000, 4))
        return 1;
    if (test__clzsi2(0x09000000, 4))
        return 1;
    if (test__clzsi2(0x0A000000, 4))
        return 1;
    if (test__clzsi2(0x0B000000, 4))
        return 1;
    if (test__clzsi2(0x0C000000, 4))
        return 1;
    if (test__clzsi2(0x0D000000, 4))
        return 1;
    if (test__clzsi2(0x0E000000, 4))
        return 1;
    if (test__clzsi2(0x0F000000, 4))
        return 1;
    if (test__clzsi2(0x10000000, 3))
        return 1;
    if (test__clzsi2(0x11000000, 3))
        return 1;
    if (test__clzsi2(0x12000000, 3))
        return 1;
    if (test__clzsi2(0x13000000, 3))
        return 1;
    if (test__clzsi2(0x14000000, 3))
        return 1;
    if (test__clzsi2(0x15000000, 3))
        return 1;
    if (test__clzsi2(0x16000000, 3))
        return 1;
    if (test__clzsi2(0x17000000, 3))
        return 1;
    if (test__clzsi2(0x18000000, 3))
        return 1;
    if (test__clzsi2(0x19000000, 3))
        return 1;
    if (test__clzsi2(0x1A000000, 3))
        return 1;
    if (test__clzsi2(0x1B000000, 3))
        return 1;
    if (test__clzsi2(0x1C000000, 3))
        return 1;
    if (test__clzsi2(0x1D000000, 3))
        return 1;
    if (test__clzsi2(0x1E000000, 3))
        return 1;
    if (test__clzsi2(0x1F000000, 3))
        return 1;
    if (test__clzsi2(0x20000000, 2))
        return 1;
    if (test__clzsi2(0x21000000, 2))
        return 1;
    if (test__clzsi2(0x22000000, 2))
        return 1;
    if (test__clzsi2(0x23000000, 2))
        return 1;
    if (test__clzsi2(0x24000000, 2))
        return 1;
    if (test__clzsi2(0x25000000, 2))
        return 1;
    if (test__clzsi2(0x26000000, 2))
        return 1;
    if (test__clzsi2(0x27000000, 2))
        return 1;
    if (test__clzsi2(0x28000000, 2))
        return 1;
    if (test__clzsi2(0x29000000, 2))
        return 1;
    if (test__clzsi2(0x2A000000, 2))
        return 1;
    if (test__clzsi2(0x2B000000, 2))
        return 1;
    if (test__clzsi2(0x2C000000, 2))
        return 1;
    if (test__clzsi2(0x2D000000, 2))
        return 1;
    if (test__clzsi2(0x2E000000, 2))
        return 1;
    if (test__clzsi2(0x2F000000, 2))
        return 1;
    if (test__clzsi2(0x30000000, 2))
        return 1;
    if (test__clzsi2(0x31000000, 2))
        return 1;
    if (test__clzsi2(0x32000000, 2))
        return 1;
    if (test__clzsi2(0x33000000, 2))
        return 1;
    if (test__clzsi2(0x34000000, 2))
        return 1;
    if (test__clzsi2(0x35000000, 2))
        return 1;
    if (test__clzsi2(0x36000000, 2))
        return 1;
    if (test__clzsi2(0x37000000, 2))
        return 1;
    if (test__clzsi2(0x38000000, 2))
        return 1;
    if (test__clzsi2(0x39000000, 2))
        return 1;
    if (test__clzsi2(0x3A000000, 2))
        return 1;
    if (test__clzsi2(0x3B000000, 2))
        return 1;
    if (test__clzsi2(0x3C000000, 2))
        return 1;
    if (test__clzsi2(0x3D000000, 2))
        return 1;
    if (test__clzsi2(0x3E000000, 2))
        return 1;
    if (test__clzsi2(0x3F000000, 2))
        return 1;
    if (test__clzsi2(0x40000000, 1))
        return 1;
    if (test__clzsi2(0x41000000, 1))
        return 1;
    if (test__clzsi2(0x42000000, 1))
        return 1;
    if (test__clzsi2(0x43000000, 1))
        return 1;
    if (test__clzsi2(0x44000000, 1))
        return 1;
    if (test__clzsi2(0x45000000, 1))
        return 1;
    if (test__clzsi2(0x46000000, 1))
        return 1;
    if (test__clzsi2(0x47000000, 1))
        return 1;
    if (test__clzsi2(0x48000000, 1))
        return 1;
    if (test__clzsi2(0x49000000, 1))
        return 1;
    if (test__clzsi2(0x4A000000, 1))
        return 1;
    if (test__clzsi2(0x4B000000, 1))
        return 1;
    if (test__clzsi2(0x4C000000, 1))
        return 1;
    if (test__clzsi2(0x4D000000, 1))
        return 1;
    if (test__clzsi2(0x4E000000, 1))
        return 1;
    if (test__clzsi2(0x4F000000, 1))
        return 1;
    if (test__clzsi2(0x50000000, 1))
        return 1;
    if (test__clzsi2(0x51000000, 1))
        return 1;
    if (test__clzsi2(0x52000000, 1))
        return 1;
    if (test__clzsi2(0x53000000, 1))
        return 1;
    if (test__clzsi2(0x54000000, 1))
        return 1;
    if (test__clzsi2(0x55000000, 1))
        return 1;
    if (test__clzsi2(0x56000000, 1))
        return 1;
    if (test__clzsi2(0x57000000, 1))
        return 1;
    if (test__clzsi2(0x58000000, 1))
        return 1;
    if (test__clzsi2(0x59000000, 1))
        return 1;
    if (test__clzsi2(0x5A000000, 1))
        return 1;
    if (test__clzsi2(0x5B000000, 1))
        return 1;
    if (test__clzsi2(0x5C000000, 1))
        return 1;
    if (test__clzsi2(0x5D000000, 1))
        return 1;
    if (test__clzsi2(0x5E000000, 1))
        return 1;
    if (test__clzsi2(0x5F000000, 1))
        return 1;
    if (test__clzsi2(0x60000000, 1))
        return 1;
    if (test__clzsi2(0x61000000, 1))
        return 1;
    if (test__clzsi2(0x62000000, 1))
        return 1;
    if (test__clzsi2(0x63000000, 1))
        return 1;
    if (test__clzsi2(0x64000000, 1))
        return 1;
    if (test__clzsi2(0x65000000, 1))
        return 1;
    if (test__clzsi2(0x66000000, 1))
        return 1;
    if (test__clzsi2(0x67000000, 1))
        return 1;
    if (test__clzsi2(0x68000000, 1))
        return 1;
    if (test__clzsi2(0x69000000, 1))
        return 1;
    if (test__clzsi2(0x6A000000, 1))
        return 1;
    if (test__clzsi2(0x6B000000, 1))
        return 1;
    if (test__clzsi2(0x6C000000, 1))
        return 1;
    if (test__clzsi2(0x6D000000, 1))
        return 1;
    if (test__clzsi2(0x6E000000, 1))
        return 1;
    if (test__clzsi2(0x6F000000, 1))
        return 1;
    if (test__clzsi2(0x70000000, 1))
        return 1;
    if (test__clzsi2(0x71000000, 1))
        return 1;
    if (test__clzsi2(0x72000000, 1))
        return 1;
    if (test__clzsi2(0x73000000, 1))
        return 1;
    if (test__clzsi2(0x74000000, 1))
        return 1;
    if (test__clzsi2(0x75000000, 1))
        return 1;
    if (test__clzsi2(0x76000000, 1))
        return 1;
    if (test__clzsi2(0x77000000, 1))
        return 1;
    if (test__clzsi2(0x78000000, 1))
        return 1;
    if (test__clzsi2(0x79000000, 1))
        return 1;
    if (test__clzsi2(0x7A000000, 1))
        return 1;
    if (test__clzsi2(0x7B000000, 1))
        return 1;
    if (test__clzsi2(0x7C000000, 1))
        return 1;
    if (test__clzsi2(0x7D000000, 1))
        return 1;
    if (test__clzsi2(0x7E000000, 1))
        return 1;
    if (test__clzsi2(0x7F000000, 1))
        return 1;
    if (test__clzsi2(0x80000000, 0))
        return 1;
    if (test__clzsi2(0x81000000, 0))
        return 1;
    if (test__clzsi2(0x82000000, 0))
        return 1;
    if (test__clzsi2(0x83000000, 0))
        return 1;
    if (test__clzsi2(0x84000000, 0))
        return 1;
    if (test__clzsi2(0x85000000, 0))
        return 1;
    if (test__clzsi2(0x86000000, 0))
        return 1;
    if (test__clzsi2(0x87000000, 0))
        return 1;
    if (test__clzsi2(0x88000000, 0))
        return 1;
    if (test__clzsi2(0x89000000, 0))
        return 1;
    if (test__clzsi2(0x8A000000, 0))
        return 1;
    if (test__clzsi2(0x8B000000, 0))
        return 1;
    if (test__clzsi2(0x8C000000, 0))
        return 1;
    if (test__clzsi2(0x8D000000, 0))
        return 1;
    if (test__clzsi2(0x8E000000, 0))
        return 1;
    if (test__clzsi2(0x8F000000, 0))
        return 1;
    if (test__clzsi2(0x90000000, 0))
        return 1;
    if (test__clzsi2(0x91000000, 0))
        return 1;
    if (test__clzsi2(0x92000000, 0))
        return 1;
    if (test__clzsi2(0x93000000, 0))
        return 1;
    if (test__clzsi2(0x94000000, 0))
        return 1;
    if (test__clzsi2(0x95000000, 0))
        return 1;
    if (test__clzsi2(0x96000000, 0))
        return 1;
    if (test__clzsi2(0x97000000, 0))
        return 1;
    if (test__clzsi2(0x98000000, 0))
        return 1;
    if (test__clzsi2(0x99000000, 0))
        return 1;
    if (test__clzsi2(0x9A000000, 0))
        return 1;
    if (test__clzsi2(0x9B000000, 0))
        return 1;
    if (test__clzsi2(0x9C000000, 0))
        return 1;
    if (test__clzsi2(0x9D000000, 0))
        return 1;
    if (test__clzsi2(0x9E000000, 0))
        return 1;
    if (test__clzsi2(0x9F000000, 0))
        return 1;
    if (test__clzsi2(0xA0000000, 0))
        return 1;
    if (test__clzsi2(0xA1000000, 0))
        return 1;
    if (test__clzsi2(0xA2000000, 0))
        return 1;
    if (test__clzsi2(0xA3000000, 0))
        return 1;
    if (test__clzsi2(0xA4000000, 0))
        return 1;
    if (test__clzsi2(0xA5000000, 0))
        return 1;
    if (test__clzsi2(0xA6000000, 0))
        return 1;
    if (test__clzsi2(0xA7000000, 0))
        return 1;
    if (test__clzsi2(0xA8000000, 0))
        return 1;
    if (test__clzsi2(0xA9000000, 0))
        return 1;
    if (test__clzsi2(0xAA000000, 0))
        return 1;
    if (test__clzsi2(0xAB000000, 0))
        return 1;
    if (test__clzsi2(0xAC000000, 0))
        return 1;
    if (test__clzsi2(0xAD000000, 0))
        return 1;
    if (test__clzsi2(0xAE000000, 0))
        return 1;
    if (test__clzsi2(0xAF000000, 0))
        return 1;
    if (test__clzsi2(0xB0000000, 0))
        return 1;
    if (test__clzsi2(0xB1000000, 0))
        return 1;
    if (test__clzsi2(0xB2000000, 0))
        return 1;
    if (test__clzsi2(0xB3000000, 0))
        return 1;
    if (test__clzsi2(0xB4000000, 0))
        return 1;
    if (test__clzsi2(0xB5000000, 0))
        return 1;
    if (test__clzsi2(0xB6000000, 0))
        return 1;
    if (test__clzsi2(0xB7000000, 0))
        return 1;
    if (test__clzsi2(0xB8000000, 0))
        return 1;
    if (test__clzsi2(0xB9000000, 0))
        return 1;
    if (test__clzsi2(0xBA000000, 0))
        return 1;
    if (test__clzsi2(0xBB000000, 0))
        return 1;
    if (test__clzsi2(0xBC000000, 0))
        return 1;
    if (test__clzsi2(0xBD000000, 0))
        return 1;
    if (test__clzsi2(0xBE000000, 0))
        return 1;
    if (test__clzsi2(0xBF000000, 0))
        return 1;
    if (test__clzsi2(0xC0000000, 0))
        return 1;
    if (test__clzsi2(0xC1000000, 0))
        return 1;
    if (test__clzsi2(0xC2000000, 0))
        return 1;
    if (test__clzsi2(0xC3000000, 0))
        return 1;
    if (test__clzsi2(0xC4000000, 0))
        return 1;
    if (test__clzsi2(0xC5000000, 0))
        return 1;
    if (test__clzsi2(0xC6000000, 0))
        return 1;
    if (test__clzsi2(0xC7000000, 0))
        return 1;
    if (test__clzsi2(0xC8000000, 0))
        return 1;
    if (test__clzsi2(0xC9000000, 0))
        return 1;
    if (test__clzsi2(0xCA000000, 0))
        return 1;
    if (test__clzsi2(0xCB000000, 0))
        return 1;
    if (test__clzsi2(0xCC000000, 0))
        return 1;
    if (test__clzsi2(0xCD000000, 0))
        return 1;
    if (test__clzsi2(0xCE000000, 0))
        return 1;
    if (test__clzsi2(0xCF000000, 0))
        return 1;
    if (test__clzsi2(0xD0000000, 0))
        return 1;
    if (test__clzsi2(0xD1000000, 0))
        return 1;
    if (test__clzsi2(0xD2000000, 0))
        return 1;
    if (test__clzsi2(0xD3000000, 0))
        return 1;
    if (test__clzsi2(0xD4000000, 0))
        return 1;
    if (test__clzsi2(0xD5000000, 0))
        return 1;
    if (test__clzsi2(0xD6000000, 0))
        return 1;
    if (test__clzsi2(0xD7000000, 0))
        return 1;
    if (test__clzsi2(0xD8000000, 0))
        return 1;
    if (test__clzsi2(0xD9000000, 0))
        return 1;
    if (test__clzsi2(0xDA000000, 0))
        return 1;
    if (test__clzsi2(0xDB000000, 0))
        return 1;
    if (test__clzsi2(0xDC000000, 0))
        return 1;
    if (test__clzsi2(0xDD000000, 0))
        return 1;
    if (test__clzsi2(0xDE000000, 0))
        return 1;
    if (test__clzsi2(0xDF000000, 0))
        return 1;
    if (test__clzsi2(0xE0000000, 0))
        return 1;
    if (test__clzsi2(0xE1000000, 0))
        return 1;
    if (test__clzsi2(0xE2000000, 0))
        return 1;
    if (test__clzsi2(0xE3000000, 0))
        return 1;
    if (test__clzsi2(0xE4000000, 0))
        return 1;
    if (test__clzsi2(0xE5000000, 0))
        return 1;
    if (test__clzsi2(0xE6000000, 0))
        return 1;
    if (test__clzsi2(0xE7000000, 0))
        return 1;
    if (test__clzsi2(0xE8000000, 0))
        return 1;
    if (test__clzsi2(0xE9000000, 0))
        return 1;
    if (test__clzsi2(0xEA000000, 0))
        return 1;
    if (test__clzsi2(0xEB000000, 0))
        return 1;
    if (test__clzsi2(0xEC000000, 0))
        return 1;
    if (test__clzsi2(0xED000000, 0))
        return 1;
    if (test__clzsi2(0xEE000000, 0))
        return 1;
    if (test__clzsi2(0xEF000000, 0))
        return 1;
    if (test__clzsi2(0xF0000000, 0))
        return 1;
    if (test__clzsi2(0xF1000000, 0))
        return 1;
    if (test__clzsi2(0xF2000000, 0))
        return 1;
    if (test__clzsi2(0xF3000000, 0))
        return 1;
    if (test__clzsi2(0xF4000000, 0))
        return 1;
    if (test__clzsi2(0xF5000000, 0))
        return 1;
    if (test__clzsi2(0xF6000000, 0))
        return 1;
    if (test__clzsi2(0xF7000000, 0))
        return 1;
    if (test__clzsi2(0xF8000000, 0))
        return 1;
    if (test__clzsi2(0xF9000000, 0))
        return 1;
    if (test__clzsi2(0xFA000000, 0))
        return 1;
    if (test__clzsi2(0xFB000000, 0))
        return 1;
    if (test__clzsi2(0xFC000000, 0))
        return 1;
    if (test__clzsi2(0xFD000000, 0))
        return 1;
    if (test__clzsi2(0xFE000000, 0))
        return 1;
    if (test__clzsi2(0xFF000000, 0))
        return 1;

    if (test__clzsi2(0x00000001, 31))
        return 1;
    if (test__clzsi2(0x00000002, 30))
        return 1;
    if (test__clzsi2(0x00000004, 29))
        return 1;
    if (test__clzsi2(0x00000008, 28))
        return 1;
    if (test__clzsi2(0x00000010, 27))
        return 1;
    if (test__clzsi2(0x00000020, 26))
        return 1;
    if (test__clzsi2(0x00000040, 25))
        return 1;
    if (test__clzsi2(0x00000080, 24))
        return 1;
    if (test__clzsi2(0x00000100, 23))
        return 1;
    if (test__clzsi2(0x00000200, 22))
        return 1;
    if (test__clzsi2(0x00000400, 21))
        return 1;
    if (test__clzsi2(0x00000800, 20))
        return 1;
    if (test__clzsi2(0x00001000, 19))
        return 1;
    if (test__clzsi2(0x00002000, 18))
        return 1;
    if (test__clzsi2(0x00004000, 17))
        return 1;
    if (test__clzsi2(0x00008000, 16))
        return 1;
    if (test__clzsi2(0x00010000, 15))
        return 1;
    if (test__clzsi2(0x00020000, 14))
        return 1;
    if (test__clzsi2(0x00040000, 13))
        return 1;
    if (test__clzsi2(0x00080000, 12))
        return 1;
    if (test__clzsi2(0x00100000, 11))
        return 1;
    if (test__clzsi2(0x00200000, 10))
        return 1;
    if (test__clzsi2(0x00400000, 9))
        return 1;

   return 0;
}
