/* 
~/riscv/riscv_newlib/bin/clang++ vector-dsl.cpp -menable-experimental-extensions \
-march=rv64gcv0p10 -O0 -mllvm --riscv-v-vector-bits-min=256 -DUSE_RVV -v 
~/riscv/git/qemu/build/qemu-riscv64 -cpu rv64,v=true a.out
~/riscv/riscv_newlib/bin/riscv64-unknown-elf-objdump -d a.out|grep vmul

The following work for #undef USE_RVV
~/llvm/15.x/llvm-project/build/bin/clang++ vector-dsl.cpp -emit-llvm -S -Xclang -ast-dump

~/llvm/15.x/llvm-project/build/bin/clang-query vector-dsl.cpp --
clang-query> set traversal IgnoreUnlessSpelledInSource
clang-query> m cxxOperatorCallExpr(binaryOperation(hasOperatorName("="),hasLHS(expr(hasType(cxxRecordDecl(hasName("UVec32")))).bind("lhs")),hasRHS(expr(cxxOperatorCallExpr(binaryOperation(hasOperatorName("+"),hasLHS(expr(cxxOperatorCallExpr(binaryOperation(hasOperatorName("*"),hasLHS(expr().bind("lhs2")),hasRHS(expr().bind("rhs2"))))).bind("lhs1")),hasRHS(expr().bind("rhs1"))))).bind("rhs"))))

Match #1:

/home/cschen/git/lbt/exlbt/clang-tools/vector-dsl.cpp:154:3: note: "lhs" binds here
  A = alpha*B + C;
  ^
/home/cschen/git/lbt/exlbt/clang-tools/vector-dsl.cpp:154:7: note: "lhs1" binds here
  A = alpha*B + C;
      ^~~~~~~
/home/cschen/git/lbt/exlbt/clang-tools/vector-dsl.cpp:154:7: note: "lhs2" binds here
  A = alpha*B + C;
      ^~~~~
/home/cschen/git/lbt/exlbt/clang-tools/vector-dsl.cpp:154:7: note: "rhs" binds here
  A = alpha*B + C;
      ^~~~~~~~~~~
/home/cschen/git/lbt/exlbt/clang-tools/vector-dsl.cpp:154:17: note: "rhs1" binds here
  A = alpha*B + C;
                ^
/home/cschen/git/lbt/exlbt/clang-tools/vector-dsl.cpp:154:13: note: "rhs2" binds here
  A = alpha*B + C;
            ^
/home/cschen/git/lbt/exlbt/clang-tools/vector-dsl.cpp:154:3: note: "root" binds here
  A = alpha*B + C;
  ^~~~~~~~~~~~~~~
1 match.


More:
clang-query> m cxxOperatorCallExpr(binaryOperation(hasOperatorName("="),hasLHS(expr(hasType(cxxRecordDecl(hasName("UVec32")))).bind("lhs")),hasRHS(expr(cxxOperatorCallExpr(binaryOperation(hasOperatorName("+"),hasLHS(expr().bind("lhs1")),hasRHS(expr().bind("rhs1"))))).bind("rhs"))))
... 
Match #2:

/home/cschen/git/lbt/exlbt/clang-tools/vector-dsl.cpp:154:3: note: "lhs" binds here
  A = alpha*B + C;
  ^
/home/cschen/git/lbt/exlbt/clang-tools/vector-dsl.cpp:154:7: note: "lhs1" binds here
  A = alpha*B + C;
      ^~~~~~~
/home/cschen/git/lbt/exlbt/clang-tools/vector-dsl.cpp:154:7: note: "rhs" binds here
  A = alpha*B + C;
      ^~~~~~~~~~~
/home/cschen/git/lbt/exlbt/clang-tools/vector-dsl.cpp:154:17: note: "rhs1" binds here
  A = alpha*B + C;
                ^
/home/cschen/git/lbt/exlbt/clang-tools/vector-dsl.cpp:154:3: note: "root" binds here
  A = alpha*B + C;
  ^~~~~~~~~~~~~~~

clang-query> m cxxOperatorCallExpr(binaryOperation(hasOperatorName("*"),hasLHS(expr().bind("lhs2")),hasRHS(expr().bind("rhs2"))))
...
Match #4:

/home/cschen/git/lbt/exlbt/clang-tools/vector-dsl.cpp:154:7: note: "lhs2" binds here
  A = alpha*B + C;
      ^~~~~
...

Ref. https://clang.llvm.org/docs/LibASTMatchersReference.html#narrowing-matchers
*/

#include "vector-dsl.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#ifdef USE_RVV
#include <riscv_vector.h>
#endif

#define array_size(a) (sizeof(a) / sizeof((a)[0]))

const Precision bit16 = Bit16;

uint32_t t[33] = {
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
};

UVec32::UVec32(uint32_t *A, size_t aSize) {
  data = A;
  size = aSize;
}

UVec32& UVec32::Mul(const uint32_t Scalar) {
  static UVec32 res(t, size);
  size_t n = size;
  uint32_t *a = res.data;
  uint32_t *b = data;
#ifdef USE_RVV
  while (n > 0) {
    size_t vl = vsetvl_e32m8(n);
    vuint32m8_t vb = vle32_v_u32m8(b, vl);
    vuint32m8_t va = vmul(vb, Scalar, vl);
    vse32(a, va, vl);
    a += vl;
    b += vl;
    n -= vl;
  }
#else
  for (int i=0; i < size; i++) {
    a[i] = Scalar*b[i];
  }
#endif
  return res;
}

UVec32& UVec32::Mul(const UVec32 &arg) {
  static UVec32 res(t, size);
  size_t n = size;
  uint32_t *a = res.data;
  uint32_t *b = data;
  uint32_t *c = arg.data;
  assert(size == arg.size);
#ifdef USE_RVV
  while (n > 0) {
    size_t vl = vsetvl_e32m8(n);
    vuint32m8_t vb = vle32_v_u32m8(b, vl);
    vuint32m8_t vc = vle32_v_u32m8(c, vl);
    vuint32m8_t va = vmul(vb, vc, vl);
    vse32(a, va, vl);
    a += vl;
    b += vl;
    c += vl;
    n -= vl;
  }
#else
  for (int i=0; i < size; i++) {
    a[i] = b[i]*c[i];
  }
#endif
  return res;
}

UVec32& UVec32::operator*(const uint32_t Scalar) {
  return Mul(Scalar);
} 

UVec32& UVec32::operator*(const UVec32 &arg) {
  return Mul(arg);
} 

UVec32& UVec32::operator+(const UVec32 &arg) {
  static UVec32 res(t, size);
  size_t n = size;
  uint32_t *a = res.data;
  uint32_t *b = data;
  uint32_t *c = arg.data;
  assert(size == arg.size);
#ifdef USE_RVV
  while (n > 0) {
    size_t vl = vsetvl_e32m8(n);
    vuint32m8_t vb = vle32_v_u32m8(b, vl);
    vuint32m8_t vc = vle32_v_u32m8(c, vl);
    vuint32m8_t va = vadd(vb, vc, vl);
    vse32(a, va, vl);
    a += vl;
    b += vl;
    c += vl;
    n -= vl;
  }
#else
  for (int i=0; i < size; i++) {
    a[i] = b[i]+c[i];
  }
#endif
  return res;
}

void UVec32::Print() {
  for (int i=0; i < size; i++) {
    printf("%u ", data[i]);
  }
  printf("\n");
}

UVec32& operator*(const uint32_t Scalar, UVec32 &B) {
  return B * Scalar;
}

uint32_t gV1[33] = {
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
};

uint32_t gV2[33] = {
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
};

uint32_t gV3[33] = {
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
};

int main() {
  UVec32 A(gV1,array_size(gV1)), B(gV2,array_size(gV2)), C(gV3,array_size(gV3));
  uint32_t alpha = (uint32_t)2;
  A = B + C;
  A = B * C;
  A = alpha*B;
  A = alpha*B + C;
  uint32_t a = 0; uint32_t b = 2; uint32_t c = 3;
  a = alpha*b + c;
  printf("B: "); B.Print();
  printf("C: "); C.Print();
  printf("A: "); A.Print();
  printf("\n\n");

  // multiply and add, madd
  // A = B * C + D; 
  // Solve above by class is not efficient, class does not use madd.
  // DSL: extend clang to parsing the left and calling A = madd(B,C,D); can get better performance.

  return 0;
}
