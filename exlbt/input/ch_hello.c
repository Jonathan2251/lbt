// ~/llvm/release/build/bin/clang -target mips-unknown-linux-gnu -c hello.cpp -emit-llvm -o hello.bc
// ~/llvm/test/build/bin/llc -march=cpu0 -mcpu=cpu032I -relocation-model=static -filetype=obj ch_hello.bc -o ch_hello.cpu0.o

// start
extern int printf(const char *format, ...);

int main(void)
{
  char *ptr = "Hello world!";

  long long a = 0x100000007fffffff;
  printf("%s\n", ptr, a);
  printf("a: %llX, %llx, %lld\n", a, a, a);
  int b = 0x10000000;
  printf("b: %x, %d\n", b, b);

  return 0;
}

