// clang -c ch_move.cpp -emit-llvm -o ch_move.bc
// /Users/Jonathan/llvm/test/cmake_debug_build/bin/Debug/llc -march=cpu0 -relocation-model=pic -filetype=asm ch_move.bc -o ch_move.cpu0.s

/// start
int main()
{
  int a = 1;
  int b = 2;
  int c = 0;
  int d = 4;
  int e = 5;

  c = a + b + d + e;

  return 0;
}
