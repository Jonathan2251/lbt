
#define DATA_TYPE double

#define M 15000
#define N 15000

DATA_TYPE X[M][N];
DATA_TYPE Y[M][N];

/* Array initialization. */
static
void init_array()
{
  int i, j;
  int k = 0;

  for (i = 0; i < M; i++)
    for (j = 0; j < N; j++) {
      X[i][j] = (DATA_TYPE)(((DATA_TYPE) ((i+1)*(j+1))) / 997.0);
      Y[i][j] = (DATA_TYPE)(((DATA_TYPE) ((i+1)*(j+1))) / 973.0);
    }
}

/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_ex11_41()
{
  int i, j, k;

#pragma omp parallel for
  for (i = 0; i < M; i++)
    for (j = 0; j < N; j++) {
	  X[i][j] = X[i][j] + Y[i-1][j];
          Y[i][j] = Y[i][j] + X[i][j-1];
    }

}

int main(int argc, char** argv)
{
  kernel_ex11_41();

  return (int)X;
}

