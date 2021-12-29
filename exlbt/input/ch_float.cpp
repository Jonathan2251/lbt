// clang -I../libsoftfloat/compiler-rt/builtins -c ch_float.cpp -emit-llvm -o ch_float.bc
// ~/llvm/test/build/bin/llc -march=cpu0 -mcpu=cpu032II -relocation-model=static -filetype=asm ch_float.bc -o -
// ~/llvm/test/build/bin/llc -march=cpu0 -mcpu=cpu032II -cpu0-s32-calls=true -relocation-model=static -filetype=asm ch_float.bc -o -

// Refernce from absvsi2_test.c, absvdi2_test.c absvti2_test.c in compiler-rt-3.5.0.src/test/builtins/Unit

/// start
//#include "debug.h"

#define DOUBLE_PRECISION
#include "fp_lib.h"
#include "int_lib.h"

extern "C" int printf(const char *format, ...);
extern "C" int sprintf(char *out, const char *format, ...);

extern "C" di_int __addvdi3(di_int a, di_int b);

int test__addvdi3(di_int a, di_int b)
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
/*
  test__addvdi3(0x8000000000000000LL, -1);  // should abort
  test__addvdi3(-1, 0x8000000000000000LL);  // should abort
  test__addvdi3(1, 0x7FFFFFFFFFFFFFFFLL);  // should abort
  test__addvdi3(0x7FFFFFFFFFFFFFFFLL, 1);  // should abort
*/

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

extern "C" di_int __absvdi2(di_int a);

int test__absvdi2(di_int a)
{
  di_int x = __absvdi2(a);
  di_int expected = a;
  if (expected < 0)
    expected = -expected;
  if (x != expected || expected < 0) {
    printf("error in __absvdi2(0x%08X%08X) = %08d%08d, expected positive %08d%08d\n",
           (int)(a>>32), (int)a, (int)(x>>32), (int)x, int(expected>>32), (int)expected);
  }
  return x != expected;
}

int test_absvdi2()
{
  int res = 0;
//     if (test__absvdi2(0x8000000000000000LL))  // should abort
//         return 1;
  res |= test__absvdi2(0x0000000000000000LL);
  res |= test__absvdi2(0x0000000000000001LL);
  res |= test__absvdi2(0x0000000000000002LL);
  res |= test__absvdi2(0x7FFFFFFFFFFFFFFELL);
  res |= test__absvdi2(0x7FFFFFFFFFFFFFFFLL); // di_int t = a >> (N - 1);  (a ^ t) - t; t=0,a=0x7ff...ff
  res |= test__absvdi2(0x8000000000000001LL);
  res |= test__absvdi2(0x8000000000000002LL);
  res |= test__absvdi2(0xFFFFFFFFFFFFFFFELL);
  res |= test__absvdi2(0xFFFFFFFFFFFFFFFFLL);

  int i;
  for (i = 0; i < 100; ++i)
    if (test__absvdi2(((di_int)i << 32) | 1))
      return 1;

  return res;
}

#define SINGLE_PRECISION

extern "C" si_int __absvsi2(si_int a);

int test__absvsi2(si_int a)
{
  //printf("a = %d\n", a);
  si_int x = __absvsi2(a);
  //printf("x = %d\n\n", x);
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
  int res = 0;
//  if (test__absvsi2(0x80000000))  // should abort
//    return 1;
  res |= test__absvsi2(0x00000000);
  res |= test__absvsi2(0x00000001);
  res |= test__absvsi2(0x00000002);
  res |= test__absvsi2(0x7FFFFFFE);
  res |= test__absvsi2(0x7FFFFFFF);
  res |= test__absvsi2(0x80000001);
  res |= test__absvsi2(0x80000002);
  res |= test__absvsi2(0xFFFFFFFE); // a=0xFFFFFFFE, t=0xFFFFFFFE>>31=0xFFFFFFFF, (0xFFFFFFFE^0xFFFFFFFF)-(-1)=1-(-1)=2
  res |= test__absvsi2(0xFFFFFFFF);

  int i;
  for (i = 0; i < 100; ++i)
    if (test__absvsi2(i))
      return 1;

  return res;
}

#define QUAD_PRECISION

//current status compiler error: undefined symbol: __absvti2

//#define CRT_HAS_128BIT

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
  if (x != expected || expected < 0) {
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

//  if (test__absvti2(make_ti(0x8000000000000000LL, 0)))  // should abort
//    return 1;
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
  printf("test_absvti2(): skipped\n");
#endif
  return 0;
}

void show_result(const char *fn, int res) {
  if (res)
    printf("%s: FAIL!\n", fn);
  else 
    printf("%s: PASS!\n", fn);
}

int main() {
  int res = 0;

  res = test_addvdi3();
  show_result("test_absvdi3()", res);
  res = test_absvdi2();
  show_result("test_absvdi2()", res);
  res = test_absvsi2();
  show_result("test_absvsi2()", res);
  res = test_absvti2();
  //show_result("test_absvti2()", res);

  return 0;
}

