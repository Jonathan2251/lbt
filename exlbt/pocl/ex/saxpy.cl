__kernel
void saxpy_kernel(float alpha,
                  __global float *A,
                  __global float *B,
                  __global float *C)
{
    //Get the index of the work-item
    int index = get_global_id(0);
    C[index] = alpha* A[index] + B[index];
}
