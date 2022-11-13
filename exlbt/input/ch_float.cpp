// clang -I../compiler-rt/builtins -I../../../lbd/lbdex/input -S ch_float.cpp -emit-llvm
// ~/llvm/test/build/bin/llc -march=cpu0 -mcpu=cpu032II -relocation-model=static -filetype=asm ch_float.ll
// ~/llvm/test/build/bin/llc -march=cpu0 -mcpu=cpu032II -cpu0-s32-calls=true -relocation-model=static -filetype=asm ch_float.ll


/// start
//#include "debug.h"

extern "C" int printf(const char *format, ...);
extern "C" int sprintf(char *out, const char *format, ...);

#include "ch9_3_longlongshift.cpp"

void test_printf()
{
  char buf[80];
  long long a = 0x100000007fffffff;
  printf("a: %llX, %llx, %lld\n", a, a, a);
  int b = 0x10000000;
  printf("b: %x, %d\n", b, b);
  sprintf(buf, "b: %x, %d\n", b, b); printf("%s", buf);

  // sanitizer_printf.cpp support right-justify for num only and left-justify
  // for string only. However, I change and support right-justify for cpu0.
  char ptr[] = "Hello world!";
  char *np = 0;
  int i = 5;
  unsigned int bs = sizeof(int)*8;
  int mi;

  mi = (1 << (bs-1)) + 1;
  printf("%s\n", ptr);
  printf("printf test\n");
  printf("%s is null pointer\n", np);
  printf("%d = 5\n", i);
  printf("%d = - max int\n", mi);
  printf("char %c = 'a'\n", 'a');
  printf("hex %x = ff\n", 0xff);
  printf("hex %02x = 00\n", 0);
  printf("signed %d = unsigned %u = hex %x\n", -3, -3, -3);
  printf("%d %s(s)", 0, "message");
  printf("\n");
  printf("%d %s(s) with %%\n", 0, "message");
  sprintf(buf, "justif: \"%-10s\"\n", "left"); printf("%s", buf);
  sprintf(buf, "justif: \"%10s\"\n", "right"); printf("%s", buf);
  sprintf(buf, " 3: %04d zero padded\n", 3); printf("%s", buf);
  sprintf(buf, " 3: %-4d left justif.\n", 3); printf("%s", buf);
  sprintf(buf, " 3: %4d right justif.\n", 3); printf("%s", buf);
  sprintf(buf, "-3: %04d zero padded\n", -3); printf("%s", buf);
  sprintf(buf, "-3: %-4d left justif.\n", -3); printf("%s", buf);
  sprintf(buf, "-3: %4d right justif.\n", -3); printf("%s", buf);
}

template <class T>
T test_shift_left(T a, T b) {
  return (a << b);
}

template <class T>
T test_shift_right(T a, T b) {
  return (a >> b);
}

template <class T1, class T2, class T3>
T1 test_add(T2 a, T3 b) {
  T1 c = a + b;
  return c;
}

template <class T1, class T2, class T3>
T1 test_mul(T2 a, T3 b) {
  T1 c = a * b;
  return c;
}

template <class T1, class T2, class T3>
T1 test_div(T2 a, T3 b) {
  T1 c = a / b;
  return c;
}

bool check_result(const char* fn, long long res, long long expected) {
  printf("%s = %lld\n", fn, res);
  if (res != expected) {
    printf("\terror: result %lld, expected %lld\n", res, expected);
  }
  return (res == expected);
}

bool check_result(const char* fn, unsigned long long res, unsigned long long expected) {
  printf("%s = %llu\n", fn, res);
  if (res != expected) {
    printf("\terror: result %llu, expected %llu\n", res, expected);
  }
  return (res == expected);
}

bool check_result(const char* fn, int res, int expected) {
  printf("%s = %d\n", fn, res);
  if (res != expected) {
    printf("\terror: result %d, expected %d\n", res, expected);
  }
  return (res == expected);
}

int main() {
  long long a;
  unsigned long long b;
  int c;

  test_printf();

  a = test_longlong_shift1();
  check_result("test_longlong_shift1()", a, 289LL);

  a = test_longlong_shift2();
  check_result("test_longlong_shift2()", a, 22LL);

// call __ashldi3
  a = test_shift_left<long long>(0x12LL, 4LL); // 0x120 = 288
  check_result("test_shift_left<long long>(0x12LL, 4LL)", a, 288LL);
  
// call __ashrdi3
  a = test_shift_right<long long>(0x001666660000000a, 48LL); // 0x16 = 22
  check_result("test_shift_right<long long>(0x001666660000000a, 48LL)", a, 22LL);
  
// call __lshrdi3
  b = test_shift_right<unsigned long long>(0x001666660000000a, 48LLu); // 0x16 = 22
  check_result("test_shift_right<unsigned long long>(0x001666660000000a, 48LLu)", b, 22LLu);
  
// call __addsf3, __fixsfsi
  c = (int)test_add<float, float, float>(-2.2, 3.3); // (int)1.1 = 1
  check_result("(int)test_add<float, float, float>(-2.2, 3.3)", c, 1);
  
// call __mulsf3, __fixsfsi
  c = (int)test_mul<float, float, float>(-2.2, 3.3); // (int)-7.26 = -7
  check_result("(int)test_mul<float, float, float>(-2.2, 3.3)", c, -7);
  
// call __divsf3, __fixsfsi
  c = (int)test_div<float, float, float>(-1.8, 0.5); // (int)-3.6 = -3
  check_result("(int)test_div<float, float, float>(-1.8, 0.5)", c, -3);
  
// call __extendsfdf2, __adddf3, __fixdfsi
  c = (int)test_add<double, double, float>(-2.2, 3.3); // (int)1.1 = 1
  check_result("(int)test_add<double, double, float>(-2.2, 3.3)", c, 1);
  
// call __extendsfdf2, __adddf3, __fixdfsi
  c = (int)test_add<double, float, double>(-2.2, 3.3); // (int)1.1 = 1
  check_result("(int)test_add<double, float, double>(-2.2, 3.3)", c, 1);
  
// call __extendsfdf2, __adddf3, __fixdfsi
  c = (int)test_add<float, float, double>(-2.2, 3.3); // (int)1.1 = 1
  check_result("(int)test_add<float, float, double>(-2.2, 3.3)", c, 1);
  
// call __extendsfdf2, __muldf3, __fixdfsi
  c = (int)test_mul<double, float, double>(-2.2, 3.3); // (int)-7.26 = -7
  check_result("(int)test_mul<double, float, double>(-2.2, 3.3)", c, -7);
  
// call __extendsfdf2, __muldf3, __truncdfsf2, __fixdfsi
// ! __truncdfsf2 in truncdfsf2.c is not work for Cpu0
  c = (int)test_mul<float, float, double>(-2.2, 3.3); // (int)-7.26 = -7
  check_result("(int)test_mul<float, float, double>(-2.2, 3.3)", c, -7);
  
// call __divdf3, __fixdfsi
  c = (int)test_div<double, double, double>(-1.8, 0.5); // (int)-3.6 = -3
  check_result("(int)test_div<double, double, double>(-1.8, 0.5)", c, -3);

#if 0 // these three do call builtins  
  c = (int)test_mul<int, int, int>(-2, 3); // -6
  check_result("(int)test_mul<int, int, int>(-2, 3)", c, -6);
  
  c = (int)test_div<int, int, int>(-10, 4); // -2 <- -2*4+2, quotient:-2, remainder:2 (remainder < 4:divident)
  check_result("(int)test_div<int, int, int>(-10, 4)", c, -3);
  
  a = test_mul<long long, long long, long long>(-2LL, 3LL); // -6LL
  check_result("test_mul<long long, long long, long long>(-2LL, 3LL)", a, -6LL);
#endif

// call __divdi3,
  a = test_div<long long, long long, long long>(-10LL, 4LL); // -3
  check_result("test_div<long long, long long, long long>(-10LL, 4LL)", a, -2LL);
  
  return 0;
}

