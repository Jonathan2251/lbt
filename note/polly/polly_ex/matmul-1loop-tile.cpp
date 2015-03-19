#include <stdio.h>

#define N 1536000
#define T 32
float A[N];
float B[N];
float C[N];

int min(int a, int b)
{
  if (a < b)
    return a;
  else
    return b;
}

void init_array()
{
    int i, j;

    for (i=0; i<N; i++) {
            A[i] = (1+i%1024)/2.0;
            B[i] = (1+i%1024)/2.0;
    }
}


int main()
{
    int k, kk;

    init_array();

            for(k=0; k<N; k=k+T)
              for (kk=k;kk<=min(k+T-1,N);kk++)
                C[kk] = A[kk] * B[kk];

    return 0;
}
