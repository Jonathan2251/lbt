#include <stdio.h>

#define N 1024*1
float A[N][N];
float B[N][N];
float C[N][N];
float D[N][N];

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

#ifdef TEST
void print_array()
{
    int i, j;

    for (i=0; i<N; i++) {
        for (j=0; j<N; j++) {
            fprintf(stdout, "%lf ", C[i][j]);
            if (j%80 == 79) fprintf(stdout, "\n");
        }
        fprintf(stdout, "\n");
    }
    for (i=0; i<N; i++) {
        for (j=0; j<N; j++) {
            fprintf(stdout, "%lf ", D[i][j]);
            if (j%80 == 79) fprintf(stdout, "\n");
        }
        fprintf(stdout, "\n");
    }
}
#endif

int main()
{
    int i, j, k;
    double t_start, t_end;

    init_array();

    for(i=0; i<N; i++)  {
        for(j=0; j<N; j++)  {
            C[i][j] = 0;
            D[i][j] = 0;
            for(k=0; k<N; k++) {
                C[i][j] = C[i][j] + A[i][k] * B[k][j];
                D[i][j] = D[i][j] + A[i][k] * 2.0;
            }
        }
    }

#ifdef TEST
    print_array();
#endif
    return 0;
}
