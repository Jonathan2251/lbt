/*
#include "Unit/absvdi2_test.c"
#include "Unit/absvsi2_test.c"
#include "Unit/absvti2_test.c"
#include "Unit/adddf3vfp_test.c"
#include "Unit/addsf3vfp_test.c"
#include "Unit/addtf3_test.c"
#include "Unit/addvdi3_test.c"
#include "Unit/addvsi3_test.c"
#include "Unit/addvti3_test.c"
*/

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

void show_result(const char *fn, int res) {
  if (res)
    printf("%s: FAIL!\n", fn);
  else
    printf("%s: PASS!\n", fn);
}

int main() {
  int res = 0;

  res = absvdi2_test();
  show_result("absvdi2_test()", res);

  res = absvsi2_test();
  show_result("absvsi2_test()", res);

  res = absvti2_test();
  //show_result("absvti2_test()", res);

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

  return 0;
}

