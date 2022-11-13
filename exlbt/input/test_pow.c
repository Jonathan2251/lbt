/* ~/llvm/debug/build/bin/clang -target amdgcn-amd-amdhsa -nogpulib -emit-llvm -S test_pow.c
~/llvm/debug/build/bin/llc -march=amdgcn -mcpu=gfx900 test_pow.ll -o -
        ...
	v_log_f32_e32 v1, v0
	v_mul_legacy_f32_e32 v0, v0, v1
	v_exp_f32_e32 v0, v0

~/llvm/debug/build/bin/clang -target mips-unknown-linux-gnu -emit-llvm -S test_pow.c
~/llvm/debug/build/bin/llc -march=mips test_pow.ll -o -
         ...
        jal	powf

Copy from llvm-project/clang/test/CodeGen/libcalls.c and 
reference llvm-project/llvm/test/CodeGen/AMDGPU/fpow.ll
*/

extern float powf(float, float);

void test_pow(float a0) {
  // call float @llvm.pow.f32
  float l0 = powf(a0, a0);
}

