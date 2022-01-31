#include "debug.h"
#include <stdlib.h>

extern "C" int printf(const char *format, ...);
extern "C" int sprintf(char *out, const char *format, ...);

extern "C" int absvdi2_test();

void show_result(const char *fn, int res) {
  if (res == 1)
    printf("%s: FAIL!\n", fn);
  else if (res == 0)
    printf("%s: PASS!\n", fn);
  else if (res == -1)
    printf("%s: SKIPPED!\n", fn);
  else {
    printf("FIXME!");
    abort();
  }
}

int main() {
  int res = 0;

// pre-defined compiler macro (from llc -march=cpu0${ENDIAN} or
// clang -target cpu0${ENDIAN}-unknown-linux-gnu
#ifdef __CPU0EB__
  printf("__CPU0EB__\n");
#endif
#ifdef __CPU0EL__
  printf("__CPU0EL__\n");
#endif

  res = absvdi2_test();
  show_result("absvdi2_test()", res);

  return 0;
}
