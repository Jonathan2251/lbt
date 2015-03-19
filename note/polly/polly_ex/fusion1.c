// ~/test/polly/llvm_build/bin/clang -Xclang -load -Xclang ~/test/polly/llvm_build/lib/LLVMPolly.so -mllvm -polly -mllvm -polly-ignore-aliasing -mllvm -polly-no-tiling -O3 -emit-llvm -S fusion1.c -o -

#define N 32
float A[N];
float B[N];
float C[N];
float D[N];

void init_array()
{
    int i, j;

    for (i=0; i<N; i++) {
          A[i] = (1+i%2);
          B[i] = (1+i%2);
    }
}

int main()
{
    int i;

    for (i=0; i<N; i++)
      C[i] = A[i] * B[i];
    for (i=0; i<N; i++)
      D[i] = A[i] * 2.0;

    return 0;
}

