// clang -target mips-unknown-linux-gnu -c ch4_1_3.cpp -emit-llvm -o ch4_1_3.bc
// /usr/local/llvm/test/cmake_debug_build/bin/llc -march=cpu0 -relocation-model=pic -filetype=asm ch4_1_3.bc -o -

// /Users/Jonathan/llvm/test/cmake_debug_build/Debug/bin/llc -march=cpu0 -relocation-model=pic -filetype=asm ch4_1_3.bc -o -

/// start
unsigned int test_rotate_left()
{
  unsigned int a = 0x0f000000;
  unsigned int b = ((a << 6) | (a >> 26));
  
  return (b);
}
