// REQUIRES: cpu0-registered-target
// RUN: %clang_cc1 -triple cpu0-unknown-linux-gnu -emit-llvm %s \
// RUN:   -o - | FileCheck %s
// RUN: %clang -target cpu0-unknown-linux-gnu -emit-llvm -S %s -o - \
// RUN:   | FileCheck %s

int foo() {
  int a = 84;
  int b = 48;
// CHECK: call i32 @llvm.cpu0.gcd
  int c = __builtin_cpu0_gcd(a, b);
  return c;
}

extern int cpu0_gcd_soft(int, int);

int bar(int a, int b) {
// CHECK: call i32 @cpu0_gcd_soft
  int c = cpu0_gcd_soft(a, b);
  return c;
}
