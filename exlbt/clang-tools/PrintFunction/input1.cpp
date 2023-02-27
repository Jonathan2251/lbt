#include "input.h"

void print_function() {
  // define nodes, dependencies and params
  Type0 A, B, C, D;
  Type1 X;
  Type0 d_B[] = {A}, d_C[] = {A}, d_D[] = {B, C};

  GetFuncAndArgs(&A, X, 0, 0);
  GetFuncAndArgs(&B, X, d_B, 1);
  GetFuncAndArgs(&C, X, d_C, 1);
  GetFuncAndArgs(&D, X, d_D, 2);
}
