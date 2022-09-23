/*
~/riscv/git/llvm-project/build_riscv_newlib/bin/clang++ --gcc-toolchain=$HOME/riscv/riscv_newlib test.cpp -march=rv64g -O0 -mabi=lp64d -v
~/riscv/git/llvm-project/build_riscv_linux/bin/clang++ --gcc-toolchain=$HOME/riscv/riscv_linux --sysroot=$HOME/riscv/riscv_linux/sysroot/ --static test.cpp -march=rv64g -O0 -mabi=lp64d -v
*/

#include <iostream>

namespace N1 {
class DataNode {
public:
  int x;
  int y;
};
} // namespace


class A {
public:
  int id;
};

namespace N1 {
class B {
public:
  N1::DataNode dn;
  A a;
};

} // namespace

using namespace std;

int main() {
  N1::B b;
  b.a.id = 1;

  std::cout << "id: " << b.a.id << std::endl;

  return 0;
}
