// clang -target mips-unknown-linux-gnu -S sor.c -emit-llvm -o -
// ~/test/0618-polly/llvm_arm_build/bin/clang -O3 -mcpu=cortex-a9 -mfloat-abi=hard -mfpu=neon sor3.c -o sor3

#include <stdio.h>
#include "sor.h"

DATA_TYPE X[N+M];

/* Array initialization. */
void init_array()
{
  int i;
  int k = 0;

  for (i = 0; i < M+N; i++)
//    X[i] = ((DATA_TYPE) (i+1)*(i+1)) / 997.0;
    X[i] = ((DATA_TYPE) (i+1));
}

DATA_TYPE* kernel_sor()
{
  int i, j, k;

// i = 0
  X[0+1] = (X[0] + X[0+1] + X[0+2]) / 3;
  X[1+1] = (X[1] + X[1+1] + X[1+2]) / 3;
  X[2+1] = (X[2] + X[2+1] + X[2+2]) / 3;
  X[3+1] = (X[3] + X[3+1] + X[3+2]) / 3;
  X[4+1] = (X[4] + X[4+1] + X[4+2]) / 3;
  X[5+1] = (X[5] + X[5+1] + X[5+2]) / 3;
// i = 1
  X[0+1] = (X[0] + X[0+1] + X[0+2]) / 3;
  X[1+1] = (X[1] + X[1+1] + X[1+2]) / 3;
  X[2+1] = (X[2] + X[2+1] + X[2+2]) / 3;
  X[3+1] = (X[3] + X[3+1] + X[3+2]) / 3;
// i = 2
  X[0+1] = (X[0] + X[0+1] + X[0+2]) / 3;
  X[1+1] = (X[1] + X[1+1] + X[1+2]) / 3;
// i = 3

    for (j = 8; j < N; j+=2) {
    // i = 3
      X[j-7] = (X[j-8] + X[j-7] + X[j-6]) / 3;
      X[j-6] = (X[j-7] + X[j-6] + X[j-5]) / 3;
    // i = 2
      X[j-5] = (X[j-6] + X[j-5] + X[j-4]) / 3;
      X[j-4] = (X[j-5] + X[j-4] + X[j-3]) / 3;
    // i = 1
      X[j-3] = (X[j-4] + X[j-3] + X[j-2]) / 3;
      X[j-2] = (X[j-3] + X[j-2] + X[j-1]) / 3;
    // i = 0
      X[j-1] = (X[j-2] + X[j-1] + X[j-0]) / 3;
      X[j-0] = (X[j-1] + X[j-0] + X[j+1]) / 3;
    }

// i = 1
  X[N-1] = (X[N-2] + X[N-1] + X[N+0]) / 3;
  X[N-0] = (X[N-1] + X[N-0] + X[N+1]) / 3;
// i = 2
  X[N-3] = (X[N-4] + X[N-3] + X[N-2]) / 3;
  X[N-2] = (X[N-3] + X[N-2] + X[N-1]) / 3;
  X[N-1] = (X[N-2] + X[N-1] + X[N+0]) / 3;
  X[N-0] = (X[N-1] + X[N-0] + X[N+1]) / 3;
// i = 3
  X[N-5] = (X[N-6] + X[N-5] + X[N-4]) / 3;
  X[N-4] = (X[N-5] + X[N-4] + X[N-3]) / 3;
  X[N-3] = (X[N-4] + X[N-3] + X[N-2]) / 3;
  X[N-2] = (X[N-3] + X[N-2] + X[N-1]) / 3;
  X[N-1] = (X[N-2] + X[N-1] + X[N+0]) / 3;
  X[N-0] = (X[N-1] + X[N-0] + X[N+1]) / 3;

  return X;
}

#ifdef PRINT
void print_array()
{
    int i;

    for (i=0; i<N+M; i++) {
//    for (i=0; i<10; i++) {
      fprintf(stdout, "%lf ", X[i]);
      if (i%80 == 79) fprintf(stdout, "\n");
    }
}
#endif

int main(int argc, char** argv)
{
  init_array();
  kernel_sor();
#ifdef PRINT
  print_array();
#endif

  return (int)X;
}

