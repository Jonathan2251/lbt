#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int test() {
    char source[] = "once upon a midnight dreary...";
    char dest[512];
    memcpy(dest, source, sizeof source);
    dest[strlen(source)] = '\0';
    printf("dest: %s\n", dest);
    return 0;
}

void show_result(const char *fn, int res) {
  if (res == 1)
    printf("%s: FAIL!\n", fn);
  else if (res == 0)
    printf("%s: PASS!\n", fn);
  else if (res == -1)
    printf("%s: SKIPPED!\n", fn);
  else {
    printf("FIXME!\n");
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

  res = test();
  show_result("test()", res);

  char cmd[256];
  printf("input: \n"); // \n: send data from buffer to device

  // scanf of libc calling _read() of syscalls.cpp and hanging on it.
  // Now, with s
  scanf("%s", cmd);

  return 0;
}

