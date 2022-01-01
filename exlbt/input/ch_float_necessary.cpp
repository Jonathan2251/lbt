// clang -I../libsoftfloat/compiler-rt/builtins -I../../lbdex/input -c ch_float_necessary.cpp -emit-llvm -o ch_float_necessary.bc
// ~/llvm/test/build/bin/llc -march=cpu0 -mcpu=cpu032II -relocation-model=static -filetype=asm ch_float_necessary.bc -o -
// ~/llvm/test/build/bin/llc -march=cpu0 -mcpu=cpu032II -cpu0-s32-calls=true -relocation-model=static -filetype=asm ch_float_necessary.bc -o -


/// start
//#include "debug.h"

extern "C" int printf(const char *format, ...);
extern "C" int sprintf(char *out, const char *format, ...);

#include "../../lbdex/input/ch9_3_longlongshift.cpp"

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

template <class T>
bool check_result(const char* fn, T res, T expected) {
  printf("%s = %d\n", fn, (int)res);
  if (res != expected) {
    printf("\terror: result %d, expected %d\n", res, expected);
  }
  return (res == expected);
}

int main() {
  int a;

  a = test_longlong_shift1();
  check_result("test_longlong_shift1()", a, 289);

  a = test_longlong_shift2();
  check_result("test_longlong_shift2()", a, 22);

// call __ashldi3
  a = (int)test_shift_left<long long>(0x12, 4); // 0x120 = 288
  check_result("(int)test_shift_left<long long>(0x12, 4)", a, 288);
  
// call __ashrdi3
  a = (int)test_shift_right<long long>(0x001666660000000a, 48); // 0x16 = 22
  check_result("(int)test_shift_right<long long>(0x001666660000000a, 48)", a, 22);
  
// call __lshrdi3
  a = (int)test_shift_right<unsigned long long>(0x001666660000000a, 48); // 0x16 = 22
  check_result("(int)test_shift_right<unsigned long long>(0x001666660000000a, 48)", a, 22);
  
// call __addsf3, __fixsfsi
  a = (int)test_add<float, float, float>(-2.2, 3.3); // (int)1.1 = 1
  check_result("(int)test_add<float, float, float>(-2.2, 3.3)", a, 1);
  
// call __mulsf3, __fixsfsi
  a = (int)test_mul<float, float, float>(-2.2, 3.3); // (int)-7.26 = -7
  check_result("(int)test_mul<float, float, float>(-2.2, 3.3)", a, -7);
  
// call __divsf3, __fixsfsi
  a = (int)test_div<float, float, float>(-1.8, 0.5); // (int)-3.6 = -3
  check_result("(int)test_div<float, float, float>(-1.8, 0.5)", a, -3);
  
// call __extendsfdf2, __adddf3, __fixdfsi
  a = (int)test_add<double, double, float>(-2.2, 3.3); // (int)1.1 = 1
  check_result("(int)test_add<double, double, float>(-2.2, 3.3)", a, 1);
  
// call __extendsfdf2, __adddf3, __fixdfsi
  a = (int)test_add<double, float, double>(-2.2, 3.3); // (int)1.1 = 1
  check_result("(int)test_add<double, float, double>(-2.2, 3.3)", a, 1);
  
// call __extendsfdf2, __adddf3, __fixdfsi
  a = (int)test_add<float, float, double>(-2.2, 3.3); // (int)1.1 = 1
  check_result("(int)test_add<float, float, double>(-2.2, 3.3)", a, 1);
  
// call __extendsfdf2, __muldf3, __fixdfsi
  a = (int)test_mul<double, float, double>(-2.2, 3.3); // (int)-7.26 = -7
  check_result("(int)test_mul<double, float, double>(-2.2, 3.3)", a, -7);
  
// call __extendsfdf2, __muldf3, __truncdfsf2, __fixdfsi
// ! __truncdfsf2 in truncdfsf2.c is not work for Cpu0
  a = (int)test_mul<float, float, double>(-2.2, 3.3); // (int)-7.26 = -7
  check_result("(int)test_mul<float, float, double>(-2.2, 3.3)", a, -7);
  
// call __divdf3, __fixdfsi
  a = (int)test_div<double, double, double>(-1.8, 0.5); // (int)-3.6 = -3
  check_result("(int)test_div<double, double, double>(-1.8, 0.5)", a, -3);
  
  return 0;
}

