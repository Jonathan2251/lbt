#include <stdlib.h>

extern "C" int printf(const char *format, ...);
extern "C" int sprintf(char *out, const char *format, ...);

extern "C" int absvdi2_test();
extern "C" int absvsi2_test();
extern "C" int absvti2_test();
extern "C" int adddf3vfp_test();
extern "C" int addsf3vfp_test();
extern "C" int addvdi3_test();
extern "C" int addvsi3_test();
extern "C" int addvti3_test();
extern "C" int ashldi3_test();
extern "C" int ashlti3_test();
extern "C" int ashrdi3_test();
extern "C" int ashrti3_test();
#if 0 // atomic.c need memcmp(...)
extern "C" int atomic_test();
#endif
extern "C" int clzdi2_test();
extern "C" int clzsi2_test();
extern "C" int clzti2_test();

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

  res = absvdi2_test();
  show_result("absvdi2_test()", res);

  res = absvsi2_test();
  show_result("absvsi2_test()", res);

  res = absvti2_test();
  show_result("absvti2_test()", res);

  res = adddf3vfp_test();
  show_result("adddf3vfp_test()", res);

  res = addsf3vfp_test();
  show_result("addsf3vfp_test()", res);

  res = addvdi3_test();
  show_result("addvdi3_test()", res);

  res = addvsi3_test();
  show_result("addvsi3_test()", res);

  res = addvti3_test();
  show_result("addvti3_test()", res);

  res = ashldi3_test();
  show_result("ashldi3_test()", res);

  res = ashlti3_test();
  show_result("ashlti3_test()", res);

  res = ashrdi3_test();
  show_result("ashrdi3_test()", res);

  res = ashrti3_test();
  show_result("ashrti3_test()", res);

#if 0 // atomic.c need memcmp(...)
  res = atomic_test();
  show_result("atomic_test()", res);
#endif

  res = clzdi2_test();
  show_result("clzdi2_test()", res);

  res = clzsi2_test();
  show_result("clzsi2_test()", res);

  res = clzti2_test();
  show_result("clzti2_test()", res);

  return 0;
}

