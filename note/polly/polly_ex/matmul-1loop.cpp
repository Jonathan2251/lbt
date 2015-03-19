#include <stdio.h>

#define N 1536000
float A[N];
float B[N];
float C[N];

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
    int k;

    init_array();

            for(k=0; k<N; k++)
                C[k] = A[k] * B[k];

    return 0;
}
