#include <stdio.h>
#include <stdlib.h>

//#define GLOABL_ARRAY

/* Dead-code elimination macros. Use argc/argv for the run-time check. */
# ifndef POLYBENCH_DUMP_ARRAYS
#  define POLYBENCH_DCE_ONLY_CODE    if (argc > 42 && ! strcmp(argv[0], ""))
# else
#  define POLYBENCH_DCE_ONLY_CODE
# endif

# define polybench_prevent_dce(func)		\
  POLYBENCH_DCE_ONLY_CODE			\
  func

#define N 1536*1

#ifdef GLOABL_ARRAY
float A[N][N];
float B[N][N];
float C[N][N];

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
#else
void init_array(float A[N][N], float B[N][N])
{
    int i, j;

    for (i=0; i<N; i++) {
        for (j=0; j<N; j++) {
            A[i][j] = (1+(i*j)%1024)/2.0;
            B[i][j] = (1+(i*j)%1024)/2.0;
        }
    }
}
#endif

#if 1
void matmul(float a[N][N], float b[N][N], float c[N][N])
{
    int i, j, k;

    for(i=0; i<N; i++)  {
        for(j=0; j<N; j++)  {
            c[i][j] = 0;
            for(k=0; k<N; k++)
                c[i][j] = c[i][j] + a[i][k] * b[k][j];
        }
    }
}
#else
void matmul()
{
    int i, j, k;

    for(i=0; i<N; i++)  {
        for(j=0; j<N; j++)  {
            C[i][j] = 0;
            for(k=0; k<N; k++)
                C[i][j] = C[i][j] + A[i][k] * B[k][j];
        }
    }
}
#endif

void print_array(float C[N][N])
{
    int i, j;

    for (i=0; i<N; i++) {
        for (j=0; j<N; j++) {
            fprintf(stdout, "%lf ", C[i][j]);
            if (j%80 == 79) fprintf(stdout, "\n");
        }
        fprintf(stdout, "\n");
    }
}

int main(int argc, char** argv)
{
    int i, j, k;
    double t_start, t_end;

#ifdef GLOABL_ARRAY
    init_array();
// The pointer such as A, B, C make polly cannot optimization.
// It can be solved by pollycc -mllvm -polly -mllvm -polly-ignore-aliasing -O3 matmul.c -o matmul.polly
    matmul(A, B, C);
#else
    float *A; float *B; float *C;
    A = malloc(N*N*sizeof(float));
    B = malloc(N*N*sizeof(float));
    C = malloc(N*N*sizeof(float));
    init_array(A, B);
// The pointer such as A, B, C make polly cannot optimization.
// It can be solved by pollycc -mllvm -polly -mllvm -polly-ignore-aliasing -O3 matmul.c -o matmul.polly
    matmul(A, B, C);
#endif

    /* Prevent dead-code elimination. All live-out data must be printed
       by the function call in argument. */
    polybench_prevent_dce(print_array(C));

    return 0;
}
