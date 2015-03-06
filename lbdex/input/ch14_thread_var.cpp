// clang -target mips-unknown-linux-gnu -c ch14_thread_var.cpp -emit-llvm -std=c++11 -o ch14_thread_var.bc
// /usr/local/llvm/test/cmake_debug_build/bin/llvm-dis ch14_thread_var.bc -o -
// /usr/local/llvm/test/cmake_debug_build/bin/llc -march=cpu0 -relocation-model=pic -filetype=asm ch14_thread_var.bc -o -
// /usr/local/llvm/test/cmake_debug_build/bin/llc -march=cpu0 -relocation-model=static -filetype=asm ch14_thread_var.bc -o -

// /Users/Jonathan/llvm/test/cmake_debug_build/Debug/bin/llvm-dis ch14_thread_var.bc -o -
// /Users/Jonathan/llvm/test/cmake_debug_build/Debug/bin/llc -march=cpu0 -relocation-model=pic -filetype=asm ch14_thread_var.bc -o -
// /Users/Jonathan/llvm/test/cmake_debug_build/Debug/bin/llc -march=cpu0 -relocation-model=static -filetype=asm ch14_thread_var.bc -o -

/// start
__thread int a = 0;
thread_local int b = 0; // need option -std=c++11
int test_thread_var()
{
    a = 2;
    return a;
}

int test_thread_var_2()
{
    b = 3;
    return b;
}

