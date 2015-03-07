// Set break point at MipsEmitGPRestore.cpp:70, it will stop at mips of llc.

// clang -c ch7_3.cpp -emit-llvm -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.8.sdk/usr/include/c++/4.2.1/ -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.8.sdk/usr/include/ -o ch7_3.bc -o ch7_3.bc
// /Users/Jonathan/llvm/test/cmake_debug_build/bin/Debug/llc -march=cpu0 -relocation-model=pic -filetype=asm ch7_3.bc -o ch7_3.cpu0.s
// /Users/Jonathan/llvm/test/cmake_debug_build/bin/Debug/llc -march=mips -relocation-model=pic -filetype=asm ch7_3.bc -o ch7_3.mips.s

/// start
#include <vector>

using namespace std;

int gI = 100;

int sum_i(int x1, int x2, int x3, int x4, int x5, int x6)
{
	int sum = gI + x1 + x2 + x3 + x4 + x5 + x6;
	
	return sum; 
}

int main()
{
	vector<int> v;
	
	try {
		v.push_back(2);
	}
	catch (...) {
		return 1;
	}
	
	int a = sum_i(1, 2, 3, 4, 5, 6);
	
	
	return a;
}
