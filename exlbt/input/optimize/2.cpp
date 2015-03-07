// clang -O3 -target mips-unknown-linux-gnu -c 2.cpp -emit-llvm -o 2.bc

/// start
extern int callee(const int *X);

int caller() {
  int T;

  T = 4;

  return callee(&T);
}

