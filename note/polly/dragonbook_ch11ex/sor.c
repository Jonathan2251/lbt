/* 
For SMP, expect the kernel_sor() transformed to:
for (i = 0; i <= m; i++)
    for (j = i; j <= i+n; j++) 
         X[j-i+1] = 1/3 * (X[j-i] + X[j-i+1] + X[j-i+2];

For superscalar, expect the kernel_sor() transformed to:
for (i = 0; i <= m; i++)
    for (j = 2*i; j <= 2*i+n; j++) 
         X[j-2*i+1] = 1/3 * (X[j-2*i] + X[j-2*i+1] + X[j-2*i+2];
*/

#define DATA_TYPE int

#define M 4
#define N 100000
#define P N/M

DATA_TYPE X[N+M];

/* Array initialization. */
static
void init_array()
{
  int i, j;
  int k = 0;

  for (i = 0; i < M; i++)
    for (j = 0; j < (N/M); j++) {
      X[k] = ((DATA_TYPE) (i+1)*(j+1)) / 997.0;
      k++;
    }
}

/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_sor()
{
  int i, j, k;

#pragma omp parallel for
  for (i = 0; i < M; i++)
    for (j = 0; j < N; j++)
	  X[j+1] = 1/3 * (X[j] + X[j+1] + X[j+2]);

}

int main(int argc, char** argv)
{
  kernel_sor();

  return (int)X;
}
