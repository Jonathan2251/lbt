// pass: 
/* ~/riscv/riscv_newlib/bin/clang++ vadd2.c -menable-experimental-extensions \
 -march=rv64gcv0p10 -O0 -mllvm --riscv-v-vector-bits-min=256 -v */
// ~/riscv/git/qemu/build/qemu-riscv64 -cpu rv64,v=true a.out
// ref. https://jia.je/software/2022/01/25/rvv-1.0-toolchain/
//      https://pages.dogdog.run/toolchain/riscv_vector_extension.html

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <riscv_vector.h>

#define array_size(a) (sizeof(a) / sizeof((a)[0]))

// Vector-vector add
void vadd(uint32_t *a, const uint32_t *b, const uint32_t *c, size_t n) {
  while (n > 0) {
    size_t vl = vsetvl_e32m8(n);
    vuint32m8_t vb = vle32_v_u32m8(b, vl);
// generate:
//   vsetvli zero, a0, e32, m8, ta, mu
//   vadd.vv v8, v8, v16
    vuint32m8_t vc = vle32_v_u32m8(c, vl);
    vuint32m8_t va = vadd(vb, vc, vl);
    vse32(a, va, vl);
    a += vl;
    b += vl;
    c += vl;
    n -= vl;
  }
}

// Vector-vector add (inline assembly)
void vadd_asm(uint32_t *a, const uint32_t *b, const uint32_t *c, size_t n) {
  while (n > 0) {
    size_t vl;
    vuint32m8_t va, vb, vc;
    //Fail: __asm__ __volatile__ ( "vsetvli %[vl], %[512], e32, m8" : [vl] "=r"(vl) : [512] "r"(512) );
    vl = vsetvl_e32m8(n);
#if (__clang_major__ > 10)
    __asm__ __volatile__ ( "vle32.v %[vb], (%[b])" : [vb] "=vr"(vb) : [b] "r"(b) : "memory" );
    __asm__ __volatile__ ( "vle32.v %[vc], (%[c])" : [vc] "=vr"(vc) : [c] "r"(c) : "memory" );
    __asm__ __volatile__ ( "vadd.vv %[va], %[vb], %[vc]" : [va] "=vr"(va) : [vb] "vr"(vb), [vc] "vr"(vc) );
    __asm__ __volatile__ ( "vse32.v %[va], (%[a])" : [va] "=vr"(va) : [a] "r"(a) : "memory" );
#else
    __asm__ __volatile__ ( "vle32.v %[vb], (%[b])" : [vb] "=v8"(vb) : [b] "r"(b) : "memory" );
    __asm__ __volatile__ ( "vle32.v %[vc], (%[c])" : [vc] "=v8"(vc) : [c] "r"(c) : "memory" );
    __asm__ __volatile__ ( "vadd.vv %[va], %[vb], %[vc]" : [va] "=v8"(va) : [vb] "v8"(vb), [vc] "v8"(vc) );
    __asm__ __volatile__ ( "vse32.v %[va], (%[a])" : [va] "=v8"(va) : [a] "r"(a) : "memory" );
#endif

    a += vl;
    b += vl;
    c += vl;
    n -= vl;
  }
}

uint32_t a[4096];
uint8_t m[512];

int main(void) {
  printf("array_size(a):%lu\n", array_size(a));
  // init source
  for (size_t i = 0; i < array_size(a); ++i)
    a[i] = i;

  vadd(a, a, a, array_size(a));

  printf("\na[]: ");
  for (size_t i = 0; i < array_size(a); ++i) {
    if (i < 10)
      printf("%d ", a[i]);
    else if (i == 11)
      printf("...");
    assert(a[i] == i * 2);
  }
  printf("\nThe results of vadd:\tPASS\n");

  vadd_asm(a, a, a, array_size(a));

  printf("\na[]: ");
  for (size_t i = 0; i < array_size(a); ++i) {
    if (i < 10)
      printf("%d ", a[i]);
    else if (i == 11)
      printf("...");
    assert(a[i] == i * 4);
  }
  printf("\nThe results of vadd_asm:\tPASS\n");

  return 0;
}

