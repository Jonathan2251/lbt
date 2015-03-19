// sudo time ./ocount --events=l1d_pend_miss:pending  ./matmul

// l1d_pend_miss: 
//   0x01: (name=pending) L1D miss oustandings duration in cycles

#include <stdio.h>

// When N is small (256 or 512), the instruction execution time is the major
// while L1 data cache miss time is minor. 
// (execution time / L1 data cache miss time) is huge.
// When N is large (20), the instruction execution time is the major
// while L1 data cache miss time is minor. 

#define N 256 // (0.018/0.018=1, 0.332/0.015=22)
/*
cschen@cschen-BM6835-BM6635-BP6335:~/install/bin$ sudo time ./ocount --events=CPU_CLK_UNHALTED,l1d_pend_miss:pending  ./matmul

Events were actively counted for 27259443 nanoseconds.
Event counts (actual) for /home/cschen/install/bin/matmul:
	Event                   Count                    % time counted
	CPU_CLK_UNHALTED        69,964,278               100.00
	l1d_pend_miss           378,939,217              100.00
0.02user 0.00system 0:00.04elapsed 48%CPU (0avgtext+0avgdata 6864maxresident)k
0inputs+0outputs (0major+4438minor)pagefaults 0swaps
cschen@cschen-BM6835-BM6635-BP6335:~/install/bin$ sudo time ./ocount --events=CPU_CLK_UNHALTED,l1d_pend_miss:pending  ./matmul.polly 

Events were actively counted for 20448161 nanoseconds.
Event counts (actual) for /home/cschen/install/bin/matmul.polly:
	Event                   Count                    % time counted
	CPU_CLK_UNHALTED        62,394,225               100.00
	l1d_pend_miss           15,619,139               100.00
0.02user 0.00system 0:00.04elapsed 46%CPU (0avgtext+0avgdata 6864maxresident)k
0inputs+0outputs (0major+4443minor)pagefaults 0swaps*/

//#define N 512 // (0.16/0.13=1.2, 3.9/1.4=2.7)

//#define N 768 // (1.35/0.42=3.2, 11.1/2.7=4.1)
//#define N 1024 // (6.62/1.21=5.4, 103.6/13.6=7.6 .. 60.1/9.1=6.7)

//#define N 1536 // (22.7/3.6=6.3, 277.5/34.8=7.9)
//#define N 2048 // (65.8/9.6=6.8, 1792.4/93.9=19.0..15727/1298=12)
//#define N 3072 // (249.8/32.4=7.7, 7867.8/333.8=23.5)
//#define N 4096 // (607.7/83.7=7.65, 17855.1/786.1=22.7) (661.5/83.9=7.8, 19326.8/793.2=24.3)
//#define N 5120 // (1235.6/149.3=8.2, 33463.1/1447.0=23.1) (1198.8/150.0=7.9, 33551.3/1469.6=22.8)


//#define DATA_TYPE int
//#define DATA_PRINTF_MODIFIER "%0.2d "
#define DATA_TYPE float
#define DATA_PRINTF_MODIFIER "%0.2lf "

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
//int l;
    int i, j, k;

    init_array();
//for (l=0; l < N; l++)
    for(i=0; i<N; i++)  {
        for(j=0; j<N; j++)  {
            C[i][j] = 0;
            for(k=0; k<N; k++)
                C[i][j] = C[i][j] + A[i][k] * B[k][j];
        }
    }

#ifdef TEST
    print_array();
#endif
    return 0;
}
