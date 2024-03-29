// ~/llvm/release/build/bin/clang -S -emit-llvm -x cl test.cl
// ~/llvm/release/build/bin/llc -filetype=asm test.ll

/* Header to make Clang compatible with OpenCL */
#define __global __attribute__((address_space(1)))
int get_global_id(int index);

/* Test kernel */
__kernel void test(__global float *in, __global float *out)
{
    int index = get_global_id(0);
    out[index] = 3.14159f * in[index] + in[index];
}
