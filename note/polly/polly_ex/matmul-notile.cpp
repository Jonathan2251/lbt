// sudo time ./ocount --events=l1d_pend_miss:pending  ./matmul

// l1d_pend_miss: 
//   0x01: (name=pending) L1D miss oustandings duration in cycles

#include <stdio.h>

#define N 1024

#define DATA_TYPE float
#define DATA_PRINTF_MODIFIER "%lf "

DATA_TYPE A[N][N];
DATA_TYPE B[N][N];
DATA_TYPE C[N][N];

void init_array()
{
    int i, j;

    for (i=0; i<N; i++) {
        for (j=0; j<N; j++) {
            A[i][j] = (1+(i*j)%1024)/2.0;
            B[i][j] = (1+(i*j)%1024)/2.0;
        }
    }
}

void print_array()
{
    int i, j;

    for (i=0; i<N; i++) {
        for (j=0; j<N; j++) {
            fprintf(stdout, DATA_PRINTF_MODIFIER, C[i][j]);
            if (j%80 == 79) fprintf(stdout, "\n");
        }
        fprintf(stdout, "\n");
    }
}

int main()
{
    int i, j, k;

    init_array();
    for(i=0; i<N; i++)  {
        for(j=0; j<N; j++)  {
            for(k=0; k<N; k++)
                C[i][j] = C[i][j] + A[i][k] * B[k][j];
        }
    }

#ifdef TEST
    print_array();
#endif
    return 0;
}
