
/// start

#include "dynamic_linker.h"

int la(int x1, int x2)
{
  int sum = x1 + x1 + x2;
  
  return sum; 
}

int foo(int x1, int x2)
{
  int sum = x1 + x2;
  
  return sum; 
}

#if 0
int factorial(int x)
{
  if (x > 0)
    return x*factorial(x-1);
  else
    return 1;
}
#endif

int bar()
{
  int a;
//  ENABLE_TRACE;
  a = foo(2, 2);
  a += la(2, 3); // 4+7=11
//  a += factorial(4); // 11+24=35

  return a;
}

