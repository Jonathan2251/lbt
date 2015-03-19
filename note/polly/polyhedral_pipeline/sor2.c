// clang -target mips-unknown-linux-gnu -S sor.c -emit-llvm -o -

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

  for (i = 0; i <= M; i++)
    for (j = 2*i; j <= 2*i+N; j++) 
      X[j-2*i+1] = (X[j-2*i] + X[j-2*i+1] + X[j-2*i+2]) / 3;

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

