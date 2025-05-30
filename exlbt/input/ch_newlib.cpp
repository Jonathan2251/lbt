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

// Currently status of my porting newlib:
// 1. newlib's printf output nothing.
// 2. scanf trigger _fstat() of syscalls.c and hanging there. 
int main() {
  int res = 0;
  char cmd[256];

//  setbuf (stdout, NULL);

// pre-defined compiler macro (from llc -march=cpu0${ENDIAN} or
// clang -target cpu0${ENDIAN}-unknown-linux-gnu
#ifdef __CPU0EB__
  printf("__CPU0EB__\n");
#endif
#ifdef __CPU0EL__
  printf("__CPU0EL__\n");
#endif
  fflush(stdout);
//  FILE * fp;

//  fscanf(stdin, "%s", cmd);

//  fp = fopen("ch_new.cpp", "r");

//  res = test();
//  show_result("test()", res);

//  printf("input: \n"); // \n: send data from buffer to device

  // scanf of libc calling _read() of syscalls.cpp and hanging on it.
  // Now, with s
//  fscanf(fp, "%s", cmd);

  return 0;
}

