
//#define DATA_TYPE int
#define DATA_TYPE float
//#define DATA_TYPE double

#define M 4
//#define N 50000000 // !! diff will not same
#define N 5000000
#define P N/M

//#define PRINTARR

#include <stdio.h>

DATA_TYPE X[N+M];

extern DATA_TYPE* kernel_sor();

/* Array initialization. */
void init_array()
{
  int i;
  int k = 0;

  for (i = 0; i < M+N; i++)
//    X[i] = ((DATA_TYPE) (i+1)*(i+1)) / 997.0;
    X[i] = ((DATA_TYPE) (i+1));
}


#ifdef PRINTARR
void print_array()
{
    int i;

    for (i=0; i<N+M; i++) {
      fprintf(stdout, "%lf ", X[i]);
      if (i%80 == 79) fprintf(stdout, "\n");
    }
}
#endif

int main(int argc, char** argv)
{
  init_array();
  kernel_sor();
#ifdef PRINTARR
  print_array();
#endif
  *argv = (char*)(X);

  return 0;
}

