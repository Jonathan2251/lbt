// clang -c ch14_eh.cpp -emit-llvm -o ch14_eh.bc
// /usr/local/llvm/test/cmake_debug_build/bin/llvm-dis ch14_eh.bc -o -
// /usr/local/llvm/test/cmake_debug_build/bin/llc -march=cpu0 -relocation-model=static -filetype=asm ch14_eh.bc -o -

// /Users/Jonathan/llvm/test/cmake_debug_build/Debug/bin/llvm-dis ch14_eh.bc -o -
// /Users/Jonathan/llvm/test/cmake_debug_build/Debug/bin/llc -march=cpu0 -relocation-model=static -filetype=asm ch14_eh.bc -o -

/// start
class Ex1 {};
void throw_exception(int a, int b) {
  Ex1 ex1;

  if (a > b) {
    throw ex1;
  }
}

int main() {
  try {
    throw_exception(2, 1);
  }
  catch(...) {
    return 1;
  }
  return 0;
}

