// clang -target mips-unknown-linux-gnu -S sor.c -emit-llvm -o -

#include "sor.hpp"

DATA_TYPE* kernel_sor()
{
  int i, j, k;

  for (i = 0; i <= M; i++)
    for (j = 2*i; j <= 2*i+N; j++) 
      X[j-2*i+1] = (X[j-2*i] + X[j-2*i+1] + X[j-2*i+2]) / 3;

  return X;
}

