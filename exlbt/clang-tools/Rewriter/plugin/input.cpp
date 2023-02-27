#include "input.h"

// A::abs(x) -> __builtin_fabs(x) if _Noreturn
// _Noreturn is just an example for adding your keyword before your DSL function.
// OpenCL uses __kernel to highlight devices' entry function.
_Noreturn double test1(double x) {
  double result =  A::
                   abs(x);
  return result;
}

_Noreturn double test2(double x) {
  return  A::abs(x);
}

double test3(double x) {
  return  A::abs(x);
}
