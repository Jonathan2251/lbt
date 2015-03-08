// clang -target `llvm-config --host-target` -c ch_all.cpp -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.8.sdk/usr/include/  -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.8.sdk/usr/include/ -emit-llvm -o ch_all.bc
// /Users/Jonathan/llvm/test/cmake_debug_build/bin/Debug/llc -march=cpu0 -relocation-model=pic -filetype=asm ch_all.bc -o ch_all.3.1.cpu0.s
// /Users/Jonathan/llvm/test/cmake_debug_build/bin/Debug/llc -march=cpu0 -relocation-model=static -filetype=asm ch_all.bc -o ch_all.3.1.cpu0.static.s
// /Users/Jonathan/llvm/test/cmake_debug_build/bin/Debug/llc -march=cpu0 -relocation-model=pic -filetype=obj ch_all.bc -o ch_all.3.1.cpu0.o

// /Users/Jonathan/llvm/test/cmake_debug_build/bin/Debug/clang -c ch_all.cpp -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.8.sdk/usr/include/  -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.8.sdk/usr/include/ -emit-llvm -o ch_all.bc
// /Users/Jonathan/llvm/test/cmake_debug_build/bin/Debug/llc -march=cpu0 -relocation-model=pic -filetype=asm ch_all.bc -o ch_all.3.2.cpu0.s
// /Users/Jonathan/llvm/test/cmake_debug_build/bin/Debug/llc -march=cpu0 -relocation-model=static -filetype=asm ch_all.bc -o ch_all.3.2.cpu0.static.s
// /Users/Jonathan/llvm/test/cmake_debug_build/bin/Debug/llc -march=cpu0 -relocation-model=pic -filetype=obj ch_all.bc -o ch_all.3.2.cpu0.o

// /Users/Jonathan/llvm/test/cmake_debug_build/bin/Debug/llc -march=cpu0 -relocation-model=pic -filetype=asm ch_all.bc -o ch_all.3.2.mips.s
// /Users/Jonathan/llvm/test/cmake_debug_build/bin/Debug/llc -march=cpu0 -relocation-model=static -filetype=asm ch_all.bc -o ch_all.3.2.mips.static.s
// /Users/Jonathan/llvm/test/cmake_debug_build/bin/Debug/llc -march=cpu0 -relocation-model=pic -filetype=obj ch_all.bc -o ch_all.3.2.mips.o

/// start
#if 1
//#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#endif
#if 1
int test_operators()
{
  int a = 5;
  int b = 2;
  int c = 0;
  int d = 0;
  int e, f, g, h, i, j, k, l = 0;
  unsigned int a1 = -5, k1 = 0, f1 = 0;

  c = a + b;
  d = a - b;
  e = a * b;
  f = a / b;
  f1 = a1 / b;
  g = (a & b);
  h = (a | b);
  i = (a ^ b);
  j = (a << 2);
  int j1 = (a1 << 2);
  k = (a >> 2);
  k1 = (a1 >> 2);

  b = !a;
  int* p = &b;
  b = (b+1)%a;
  c = rand();
//  c = 12;
  b = (b+1)%c;
  
  return c;
}
#endif
#if 1
int gI = 100;

int test_globalvar()
{
  int c = 0;

  c = gI;
  
  return c;
}
#endif
#if 1
struct Date
{
  int year;
  int month;
  int day;
};

Date date = {2012, 10, 12};
int a[3] = {2012, 10, 12};

int test_struct()
{
  int day = date.day;
  int i = a[1];

  return 0;
}
#endif
#if 1
int test_control()
{
  int a = 3;
  
  if (a != 0)
    a++;
  goto L1;
  a++;
L1:
  a--;
    
  return a;
}
#endif
#if 1
template<class T>
T sum(T amount, ...)
{
  T i = 0;
  T val = 0;
  T sum = 0;
	
  va_list vl;
  va_start(vl, amount);
  for (i = 0; i < amount; i++)
  {
    val = va_arg(vl, T);
    sum += val;
  }
  va_end(vl);
  
  return sum; 
}
#endif
int main()
{
  int result = 0;
  result = test_operators();
  result = test_globalvar();
  result = test_struct();
  result = test_control();
  int a = sum<int>(6, 1, 2, 3, 4, 5, 6);
//  printf("a = %d\n", a);
	
  return result;
}

