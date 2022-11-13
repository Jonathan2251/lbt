// RISCV does need -lm while X86-64 does not.
// $ ~/riscv/riscv_newlib/bin/clang++ -menable-experimental-extensions pow.cpp -march=rv64gcv0p10 -O0 -fuse-ld=lld -mno-relax -g -mabi=lp64d -lm  -v
// $ ~/riscv/git/qemu/build/qemu-riscv64 -cpu rv64,v=true a.out

#include <stdio.h>
#include <math.h>

double base = 100;
double power = 2;
double test_math()
{
  double res = 0;

  res = pow(base, power);

  return res;
}

int main() {
  double a = test_math();
  printf("a = %lf\n", a);
  return 0;
}
