// ~/llvm/10.0.0/release/build-cb/bin/clang++ -Xclang -load -Xclang /home/czs/llvm/10.0.0/release/build-cb/lib/CppDslEx1.so -Xclang -add-plugin -Xclang ex1-act -Xclang -emit-llvm -S ex1.cl -o -

/* Header to make Clang compatible with OpenCL */
#define __global __attribute__((address_space(1)))
int get_global_id(int index);

/* Test kernel */
__kernel void test(__global float *in, __global float *out)
{
    int index = get_global_id(0);
    out[index] = 3.14159f * in[index] + in[index];
}

int addone(int x) {
  return  x+1;
}
