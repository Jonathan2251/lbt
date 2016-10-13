// clang -I../libsoftfloat/compiler-rt/builtins -c ch_float.cpp -emit-llvm -o ch_float.bc
// ~/llvm/test/cmake_debug_build/Debug/bin/llc -march=cpu0 -mcpu=cpu032II -relocation-model=static -filetype=asm ch_float.bc -o -
// ~/llvm/test/cmake_debug_build/bin/llc -march=cpu0 -mcpu=cpu032II -relocation-model=static -filetype=asm ch_float.bc -o -
// ~/llvm/test/cmake_debug_build/bin/llc -march=cpu0 -mcpu=cpu032II -cpu0-s32-calls=true -relocation-model=static -filetype=asm ch_float.bc -o -


/// start
#include "debug.h"

#define SINGLE_PRECISION
#include "fp_lib.h"

extern "C" int __fixsfsi(fp_t a);
extern "C" int __fixdfsi(double a);

extern long long test_longlong_shift1();
extern long long test_longlong_shift2();

#include "print.cpp"

int test_float_to_int()
{
  float a = -2.2;
  si_int c = __fixsfsi(a);

  return c;
}

// work
int test_double_to_int()
{
  double a = -4.2;
  si_int c = __fixdfsi(a);

  return c;
}

#include "int_lib.h"

extern "C" int printf(const char *format, ...);
extern "C" int sprintf(char *out, const char *format, ...);

int test_float_add()
{
  float a = -2.2;
  float b = 3.3;
  
  int c = (int)(a + b); // 1

  return c;
}

int test_float_mul()
{
  float a = -2.2;
  float b = 3.3;
  float c = -4.3;
  
  int d = (int)(a * b * c); // 31.218=31

  return d;
}

int test_float_div()
{
  float a = -2.2;
  float b = 3.3;
  float c = 1.2;
  
  int d = (int)((a * b) / c); // -6

  return d;
}

int test_double_to_int_2()
{
  double b = 3.3;
  
  int c = (int)(b);

  return c;
}

int test_double_add()
{
  double a = 2.2;
  double b = 3.3;

  int c = (int)(a + b);

  return c;
}

int test_double_mul()
{
  double a = -2.2;
  double b = 3.3;
  double c = 4.3;
  
  int d = (int)(a * b * c); // -31.218=-31

  return d;
}

int test_double_div()
{
  double a = -2.2;
  double b = 3.3;
  double c = 1.2;
  
  int d = (int)((a * b) / c); // -6

  return d;
}

#if 0
//extern "C" di_int __addvdi3(di_int a, di_int b);
extern "C" di_int __addvdi3(long long a, long long b);

//int test__addvdi3(di_int a, di_int b)
int test__addvdi3(long long a, long long b)
{
    di_int x = __addvdi3(a, b);
    di_int expected = a + b;
    if (x != expected)
        printf("error in test__addvdi3(0x%llX, 0x%llX) = %lld, expected %lld\n",
                a, b, x, expected);
    return x != expected;
}

int test_addvdi3()
{
//     test__addvdi3(0x8000000000000000LL, -1);  // should abort
//     test__addvdi3(-1, 0x8000000000000000LL);  // should abort
//     test__addvdi3(1, 0x7FFFFFFFFFFFFFFFLL);  // should abort
//     test__addvdi3(0x7FFFFFFFFFFFFFFFLL, 1);  // should abort

    if (test__addvdi3(0x8000000000000000LL, 1))
        return 1;
    if (test__addvdi3(1, 0x8000000000000000LL))
        return 1;
    if (test__addvdi3(0x8000000000000000LL, 0))
        return 1;
    if (test__addvdi3(0, 0x8000000000000000LL))
        return 1;
    if (test__addvdi3(0x7FFFFFFFFFFFFFFLL, -1))
        return 1;
    if (test__addvdi3(-1, 0x7FFFFFFFFFFFFFFLL))
        return 1;
    if (test__addvdi3(0x7FFFFFFFFFFFFFFFLL, 0))
        return 1;
    if (test__addvdi3(0, 0x7FFFFFFFFFFFFFFFLL))
        return 1;

    return 0;
}
#endif


extern "C" di_int __absvdi2(di_int a);

int test__absvdi2(di_int a)
{
    di_int x = __absvdi2(a);
    di_int expected = a;
    if (expected < 0)
        expected = -expected;
    if (x != expected || expected < 0)
        printf("error in __absvdi2(0x%08X%08X) = %08d%08d, expected positive %08d%08d\n",
               (int)(a>>32), (int)a, (int)(x>>32), (int)x, int(expected>>32), (int)expected);
    return x != expected;
}

int test_absvdi2()
{
//     if (test__absvdi2(0x8000000000000000LL))  // should abort
//         return 1;
    test__absvdi2(0x0000000000000000LL);
    test__absvdi2(0x0000000000000001LL);
    test__absvdi2(0x0000000000000002LL);
    test__absvdi2(0x7FFFFFFFFFFFFFFELL);
    test__absvdi2(0x7FFFFFFFFFFFFFFFLL);
    test__absvdi2(0x8000000000000001LL);
    test__absvdi2(0x8000000000000002LL);
    test__absvdi2(0xFFFFFFFFFFFFFFFELL);
    test__absvdi2(0xFFFFFFFFFFFFFFFFLL);

    int i;
    for (i = 0; i < 100; ++i)
        if (test__absvdi2(((di_int)i << 32) | 1))
            return 1;

    return 0;
}

extern "C" si_int __absvsi2(si_int a);

int test__absvsi2(si_int a)
{
    si_int x = __absvsi2(a);
    si_int expected = a;
    if (expected < 0)
        expected = -expected;
    if (x != expected || expected < 0)
        printf("error in __absvsi2(0x%X) = %d, expected positive %d\n",
               a, x, expected);
    return x != expected;
}

int test_absvsi2()
{
//     if (test__absvsi2(0x80000000))  // should abort
//         return 1;
    test__absvsi2(0x00000000);
    test__absvsi2(0x00000001);
    test__absvsi2(0x00000002);
    test__absvsi2(0x7FFFFFFE);
    test__absvsi2(0x7FFFFFFF);
    test__absvsi2(0x80000001);
    test__absvsi2(0x80000002);
    test__absvsi2(0xFFFFFFFE);
    test__absvsi2(0xFFFFFFFF);

    int i;
    for (i = 0; i < 100; ++i)
        if (test__absvsi2(i))
            return 1;

    return 0;
}

#if 0
#define CRT_HAS_128BIT

#ifdef CRT_HAS_128BIT

// Returns: absolute value

// Effects: aborts if abs(x) < 0

extern "C" ti_int __absvti2(ti_int a);

int test__absvti2(ti_int a)
{
    ti_int x = __absvti2(a);
    ti_int expected = a;
    if (expected < 0)
        expected = -expected;
    if (x != expected || expected < 0)
    {
        twords at;
        at.all = a;
        twords xt;
        xt.all = x;
        twords expectedt;
        expectedt.all = expected;
        printf("error in __absvti2(0x%8X%8X.%8X%8X) = "
               "0x%8X%8X%.8X%.8X, expected positive 0x%8X%8X%.8X%.8X\n",
               (int)(at.s.high>>32), (int)(at.s.high), (int)(at.s.low>>32), 
               (int)(at.s.low), (int)(xt.s.high>>32), (int)(xt.s.high), 
               (int)(xt.s.low>>32), (int)(xt.s.low),
               (int)(expectedt.s.high>>32), (int)(expectedt.s.high), 
               (int)(expectedt.s.low>>32), (int)(expectedt.s.low));
    }
    return x != expected;
}

#endif

int test_absvti2()
{
#ifdef CRT_HAS_128BIT

//     if (test__absvti2(make_ti(0x8000000000000000LL, 0)))  // should abort
//         return 1;
    if (test__absvti2(0x0000000000000000LL))
        return 1;
    if (test__absvti2(0x0000000000000001LL))
        return 1;
    if (test__absvti2(0x0000000000000002LL))
        return 1;
    if (test__absvti2(make_ti(0x7FFFFFFFFFFFFFFFLL, 0xFFFFFFFFFFFFFFFELL)))
        return 1;
    if (test__absvti2(make_ti(0x7FFFFFFFFFFFFFFFLL, 0xFFFFFFFFFFFFFFFFLL)))
        return 1;
    if (test__absvti2(make_ti(0x8000000000000000LL, 0x0000000000000001LL)))
        return 1;
    if (test__absvti2(make_ti(0x8000000000000000LL, 0x0000000000000002LL)))
        return 1;
    if (test__absvti2(make_ti(0xFFFFFFFFFFFFFFFFLL, 0xFFFFFFFFFFFFFFFELL)))
        return 1;
    if (test__absvti2(make_ti(0xFFFFFFFFFFFFFFFFLL, 0xFFFFFFFFFFFFFFFFLL)))
        return 1;

    int i;
    for (i = 0; i < 10000; ++i)
        if (test__absvti2(make_ti(((ti_int)i << 32) | i,
                                  ((ti_int)i << 32) | i)))
            return 1;
#else
    printf("skipped\n");
#endif
    return 0;
}
#endif

int main() {
  int a;

  a = test_longlong_shift1(); // 0x121 = 289
  printf("test_longlong_shift1() = %d\n", a);
  a = test_longlong_shift2(); // 0x16 = 22
  printf("test_longlong_shift2() = %d\n", a);
  a = test_float_to_int(); // -2
  printf("test_float_to_int() = %d\n", a);
  a = test_double_to_int(); // -4
  printf("test_double_to_int() = %d\n", a);
  a = test_double_to_int_2(); // 3
  printf("test_double_to_int_2() = %d\n", a);
  a = test_float_add(); // 1
  printf("test_float_add() = %d\n", a);
  a = test_float_mul(); // 31
  printf("test_float_mul() = %d\n", a);
  a = test_float_div(); // -6
  printf("test_float_div() = %d\n", a);
  a = test_double_add(); // 5
  printf("test_double_add() = %d\n", a);
  a = test_double_mul(); // -31
  printf("test_double_mul() = %d\n", a);
  a = test_double_div(); // -6
  printf("test_double_div() = %d\n", a);

#if 0
  test_addvdi3();
#endif
#if 0 // fail
  test_absvdi2();
  test_absvsi2();
#endif

  return 0;
}

