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

// atomic.c need memcmp(...)
//extern "C" int atomic_test();

extern "C" int clzdi2_test();
extern "C" int clzsi2_test();
extern "C" int clzti2_test();
extern "C" int cmpdi2_test();
extern "C" int cmpti2_test();
extern "C" int comparedf2_test();
extern "C" int comparesf2_test();

// Needless to compare compiler_rt_logb() with logb() of libm
//extern "C" int compiler_rt_logb_test();
//extern "C" int compiler_rt_logbf_test();
//extern "C" int compiler_rt_logbl_test();

extern "C" int cpu_model_test();
extern "C" int ctzdi2_test();
extern "C" int ctzsi2_test();
extern "C" int ctzti2_test();

// div for complex type need libm: cabs, isinf, ..., skip it at this point
//extern "C" int divdc3_test();
extern "C" int divdf3_test();
extern "C" int divdf3vfp_test();
extern "C" int divdi3_test();
extern "C" int divmodsi4_test();
extern "C" int divmodti4_test();
//extern "C" int divsc3_test();
extern "C" int divsf3_test();
extern "C" int divsf3vfp_test();
extern "C" int divsi3_test();
//extern "C" int divtc3_test();
extern "C" int divtf3_test();
extern "C" int divti3_test();
//extern "C" int divxc3_test();

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

  res = cmpdi2_test();
  show_result("cmpdi2_test()", res);

  res = cmpti2_test();
  show_result("cmpti2_test()", res);

  res = comparedf2_test();
  show_result("comparedf2_test()", res);

  res = comparesf2_test();
  show_result("comparesf2_test()", res);

//  res = compiler_rt_logb_test();
//  show_result("compiler_rt_logb_test()", res);

//  res = compiler_rt_logbf_test();
//  show_result("compiler_rt_logbf_test()", res);

//  res = compiler_rt_logbl_test();
//  show_result("compiler_rt_logbl_test()", res);

  res = cpu_model_test();
  show_result("cpu_model_test()", res);

  res = ctzdi2_test();
  show_result("ctzdi2_test()", res);

  res = ctzsi2_test();
  show_result("ctzsi2_test()", res);

  res = ctzti2_test();
  show_result("ctzti2_test()", res);

//  res = divdc3_test();
//  show_result("divdc3_test()", res);

  res = divdf3_test();
  show_result("divdf3_test()", res);

  res = divdf3vfp_test();
  show_result("divdf3vfp_test()", res);

  res = divdi3_test();
  show_result("divdi3_test()", res);

  res = divmodsi4_test();
  show_result("divmodsi4_test()", res);

  res = divmodti4_test();
  show_result("divmodti4_test()", res);

//  res = divsc3_test();
//  show_result("divsc3_test()", res);

  res = divsf3_test();
  show_result("divsf3_test()", res);

  res = divsf3vfp_test();
  show_result("divsf3vfp_test()", res);

  res = divsi3_test();
  show_result("divsi3_test()", res);

//  res = divtc3_test();
//  show_result("divtc3_test()", res);

  res = divtf3_test();
  show_result("divtf3_test()", res);

  res = divti3_test();
  show_result("divti3_test()", res);

//  res = divxc3_test();
//  show_result("divxc3_test()", res);

  return 0;
}

