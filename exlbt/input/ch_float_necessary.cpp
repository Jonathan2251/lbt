// clang -I../libsoftfloat/compiler-rt/builtins -I../../lbdex/input -c ch_float_necessary.cpp -emit-llvm -o ch_float_necessary.bc
// ~/llvm/test/cmake_debug_build/Debug/bin/llc -march=cpu0 -mcpu=cpu032II -relocation-model=static -filetype=asm ch_float_necessary.bc -o -
// ~/llvm/test/cmake_debug_build/bin/llc -march=cpu0 -mcpu=cpu032II -relocation-model=static -filetype=asm ch_float_necessary.bc -o -
// ~/llvm/test/cmake_debug_build/bin/llc -march=cpu0 -mcpu=cpu032II -cpu0-s32-calls=true -relocation-model=static -filetype=asm ch_float_necessary.bc -o -


/// start
//#include "debug.h"

extern "C" int printf(const char *format, ...);
extern "C" int sprintf(char *out, const char *format, ...);

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

int main() {
  int a;

// call __ashldi3
  a = (int)test_shift_left<long long>(0x12, 4); // 0x120 = 288
  printf("(int)test_shift_left<long long>(0x12, 4) = %d\n", a);
  
// call __ashrdi3
  a = (int)test_shift_right<long long>(0x001666660000000a, 48); // 0x16 = 22
  printf("(int)test_shift_right<long long>(0x001666660000000a, 48) = %d\n", a);
  
// call __lshrdi3
  a = (int)test_shift_right<unsigned long long>(0x001666660000000a, 48); // 0x16 = 22
  printf("(int)test_shift_right<unsigned long long>(0x001666660000000a, 48) = %d\n", a);
  
// call __addsf3, __fixsfsi
  a = (int)test_add<float, float, float>(-2.2, 3.3); // (int)1.1 = 1
  printf("(int)test_add<float, float, float>(-2.2, 3.3) = %d\n", a);
  
// call __mulsf3, __fixsfsi
  a = (int)test_mul<float, float, float>(-2.2, 3.3); // (int)-7.26 = -7
  printf("(int)test_mul<float, float, float>(-2.2, 3.3) = %d\n", a);
  
// call __divsf3, __fixsfsi
  a = (int)test_div<float, float, float>(-1.8, 0.5); // (int)-3.6 = -3
  printf("(int)test_div<float, float, float>(-1.8, 0.5) = %d\n", a);
  
// call __extendsfdf2, __adddf3, __fixdfsi
  a = (int)test_add<double, double, float>(-2.2, 3.3); // (int)1.1 = 1
  printf("(int)test_add<double, double, float>(-2.2, 3.3) = %d\n", a);
  
// call __extendsfdf2, __muldf3, __fixdfsi
  a = (int)test_mul<double, float, double>(-2.2, 3.3); // (int)-7.26 = -7
  printf("(int)test_mul<double, float, double>(-2.2, 3.3) = %d\n", a);
  
// call __extendsfdf2, __muldf3, __truncdfsf2, __fixdfsi
// ! __truncdfsf2 in truncdfsf2.c is not work for Cpu0
  a = (int)test_mul<float, float, double>(-2.2, 3.3); // (int)-7.26 = -7
  printf("(int)test_mul<float, float, double>(-2.2, 3.3) = %d\n", a);
  
// call __divdf3, __fixdfsi
  a = (int)test_div<double, double, double>(-1.8, 0.5); // (int)-3.6 = -3
  printf("(int)test_div<double, double, double>(-1.8, 0.5) = %d\n", a);
  
  return 0;
}

