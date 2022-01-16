// ~/llvm/release/build/bin/clang -target mips-unknown-linux-gnu -c hello.cpp -emit-llvm -o hello.bc
// ~/llvm/test/build/bin/llc -march=cpu0 -mcpu=cpu032I -relocation-model=static -filetype=obj ch_hello.bc -o ch_hello.cpu0.o

// start
extern int printf(const char *format, ...);

int main(void)
{
  char *ptr = "Hello world!";

#if 1
  long long a = 0x100000007fffffff;
  printf("%s:%lld\n", ptr, a);
#else
  //int a = 0x10000000;
  int a = 0x10;
  printf("%s:%d\n", ptr, a);
#endif
  return 0;
}

