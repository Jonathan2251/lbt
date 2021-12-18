// REQUIRES: cpu0-registered-target
// RUN: %clang_cc1 -triple cpu0-unknown-linux-gnu -emit-llvm %s \
// RUN:            -o - \
// RUN:   | FileCheck %s


int foo() {
  int a = 84;
  int b = 48;
// CHECK: call i32 @llvm.cpu0.gcd
  int c = __builtin_cpu0_gcd(a, b);
  return c;
}
