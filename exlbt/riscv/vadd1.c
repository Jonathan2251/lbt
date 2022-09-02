/* ~/riscv/riscv_newlib/bin/clang++ vadd1.c -menable-experimental-extensions \
 -march=rv64gcv0p10 -O0 -mllvm --riscv-v-vector-bits-min=256 -v */
// ~/riscv/git/qemu/build/qemu-riscv64 -cpu rv64,v=true a.out
// ~/riscv/riscv_newlib/bin/riscv64-unknown-elf-objdump -d a.out|grep vadd
// ref. https://pages.dogdog.run/toolchain/riscv_vector_extension.html

// ~/Andes/riscv/riscv_newlib/bin/clang++ vadd1.c -menable-experimental-extensions  -march=rv64gcv0p10 -menable-experimental-extensions -O0 -v

// pass: 
// ~/Andes/riscv/riscv_newlib/bin/clang++ vadd1.c -march=rv64imfv0p10zfh0p1 -menable-experimental-extensions -mabi=lp64d -O0 -v
// ~/riscv/riscv_newlib/bin/clang++ vadd1.c -march=rv64imfv0p10zfh0p1 -menable-experimental-extensions -mabi=lp64d -O0 -v
// ~/riscv/riscv_newlib/bin/llvm-readelf -h a.out
// Flags:                             0x5, RVC, double-float ABI
//
// fail: ~/Andes/riscv/riscv_newlib/bin/clang++ vadd1.c -march=rv64imfv0p10zfh0p1 -menable-experimental-extensions –mabi=lp64f -O0 -v

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <riscv_vector.h>

#define array_size(a) (sizeof(a) / sizeof((a)[0]))

enum {VADD, VMUL};

// Vector-vector add
void vOp(uint32_t *a, const uint32_t *b, const uint32_t *c, size_t n, int VectorOp) {
  while (n > 0) {
    size_t vl = vsetvl_e32m8(n);
    vuint32m8_t va;
    vuint32m8_t vb = vle32_v_u32m8(b, vl);
// generate:
//   vsetvli zero, a0, e32, m8, ta, mu
    vuint32m8_t vc = vle32_v_u32m8(c, vl);
    if (VectorOp == VADD) {
      // vadd.vv v8, v8, v16
      va = vadd(vb, vc, vl);
    }
    else if (VectorOp == VMUL) {
      // vmul.vv v8, v8, v16
      va = vmul(vb, vc, vl);
    }
    vse32(a, va, vl);
    a += vl;
    b += vl;
    c += vl;
    n -= vl;
  }
}

uint32_t a[4096];
//uint32_t a[48];
//uint8_t m[512];

int main(void) {
  size_t vl = vsetvl_e32m8(array_size(a));
  printf("vl: %lu\n", vl);
  printf("array_size(a):%lu\n", array_size(a));

  // init source
  for (size_t i = 0; i < array_size(a); ++i)
    a[i] = i;

  vOp(a, a, a, array_size(a), VADD);

  printf("\na[]: ");
  for (size_t i = 0; i < array_size(a); ++i) {
    if (i < 64)
      printf("%d ", a[i]);
    else if (i == 64)
      printf("...");
    assert(a[i] == i * 2);
  }
  printf("\nThe results of VADD:\tPASS\n");

  vOp(a, a, a, array_size(a), VMUL);

  printf("\na[]: ");
  for (size_t i = 0; i < array_size(a); ++i) {
    if (i < 64)
      printf("%d ", a[i]);
    else if (i == 64)
      printf("...");
    assert(a[i] == (i*2)*(i*2));
  }
  printf("\nThe results of VMUL:\tPASS\n");

  return 0;
}

