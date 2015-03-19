// clang -S sor.c -emit-llvm -o -
// ~/test/0618-polly/llvm_arm_build/bin/clang -O3 -mcpu=cortex-a9 -mfloat-abi=hard -mfpu=neon sor.c -o sor

#include "sor.hpp"

DATA_TYPE* kernel_sor()
{
  int i, j, k;

  for (i = 0; i < M; i++)
    for (j = 0; j < N; j++)
      X[j+1] = (X[j] + X[j+1] + X[j+2]) / 3;

  return X;
}

