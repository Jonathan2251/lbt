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
extern "C" int bswapdi2_test();
extern "C" int bswapsi2_test();

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
#ifdef HAS_COMPLEX
extern "C" int divdc3_test();
#endif
extern "C" int divdf3_test();
extern "C" int divdf3vfp_test();
extern "C" int divdi3_test();
extern "C" int divmodsi4_test();
extern "C" int divmodti4_test();
#ifdef HAS_COMPLEX
extern "C" int divsc3_test();
#endif
extern "C" int divsf3_test();
extern "C" int divsf3vfp_test();
extern "C" int divsi3_test();
#ifdef HAS_COMPLEX
extern "C" int divtc3_test();
#endif
extern "C" int divtf3_test();
extern "C" int divti3_test();
#ifdef HAS_COMPLEX
extern "C" int divxc3_test();
#endif
extern "C" int enable_execute_stack_test();
extern "C" int eqdf2vfp_test();
extern "C" int eqsf2vfp_test();
extern "C" int eqtf2_test();
extern "C" int extenddftf2_test();
extern "C" int extendhfsf2_test();
extern "C" int extendhftf2_test();
extern "C" int extendsfdf2vfp_test();
extern "C" int extendsftf2_test();
#if 0
extern "C" int gcc_personality_test();
#endif
extern "C" int gedf2vfp_test();
extern "C" int gesf2vfp_test();
extern "C" int getf2_test();
extern "C" int gtdf2vfp_test();
extern "C" int gtsf2vfp_test();
extern "C" int gttf2_test();
extern "C" int ledf2vfp_test();
extern "C" int lesf2vfp_test();
extern "C" int letf2_test();
extern "C" int lshrdi3_test();
extern "C" int lshrti3_test();
extern "C" int ltdf2vfp_test();
extern "C" int ltsf2vfp_test();
extern "C" int lttf2_test();
extern "C" int moddi3_test();
extern "C" int modsi3_test();
extern "C" int modst3_test();
extern "C" int modti3_test();
#ifdef HAS_COMPLEX
extern "C" int muldc3_test();
#endif
extern "C" int muldf3vfp_test();
extern "C" int muldi3_test();
extern "C" int mulodi4_test();
extern "C" int mulosi4_test();
extern "C" int muloti4_test();
#ifdef HAS_COMPLEX
extern "C" int mulsc3_test();
#endif
extern "C" int mulsf3vfp_test();
//extern "C" int mulsi3_test(); no this mulsi3.c
extern "C" int multf3_test();
extern "C" int multi3_test();
extern "C" int mulvdi3_test();
extern "C" int mulvsi3_test();
extern "C" int mulvti3_test();
#ifdef HAS_COMPLEX
extern "C" int mulxc3_test();
#endif

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

  res = bswapdi2_test();
  show_result("bswapdi2_test()", res);

  res = bswapsi2_test();
  show_result("bswapsi2_test()", res);

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

#ifdef HAS_COMPLEX
  res = divdc3_test();
  show_result("divdc3_test()", res);
#endif

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

#ifdef HAS_COMPLEX
  res = divsc3_test();
  show_result("divsc3_test()", res);
#endif

  res = divsf3_test();
  show_result("divsf3_test()", res);

  res = divsf3vfp_test();
  show_result("divsf3vfp_test()", res);

  res = divsi3_test();
  show_result("divsi3_test()", res);

#ifdef HAS_COMPLEX
  res = divtc3_test();
  show_result("divtc3_test()", res);
#endif

  res = divtf3_test();
  show_result("divtf3_test()", res);

  res = divti3_test();
  show_result("divti3_test()", res);

#ifdef HAS_COMPLEX
  res = divxc3_test();
  show_result("divxc3_test()", res);
#endif

#if 0
  res = enable_execute_stack_test();
  show_result("enable_execute_stack_test()", res);
#endif

  res = eqdf2vfp_test();
  show_result("eqdf2vfp_test()", res);

  res = eqsf2vfp_test();
  show_result("eqsf2vfp_test()", res);

  res = eqtf2_test();
  show_result("eqtf2_test()", res);

  res = extenddftf2_test();
  show_result("extenddftf2_test()", res);

  res = extendhfsf2_test();
  show_result("extendhfsf2_test()", res);

  res = extendhftf2_test();
  show_result("extendhftf2_test()", res);

  res = extendsfdf2vfp_test();
  show_result("extendsfdf2vfp_test()", res);

  res = extendsftf2_test();
  show_result("extendsftf2_test()", res);

#if 0
  res = gcc_personality_test();
  show_result("gcc_personality_test()", res);
#endif

  res = gedf2vfp_test();
  show_result("gedf2vfp_test()", res);

  res = gesf2vfp_test();
  show_result("gesf2vfp_test()", res);

  res = getf2_test();
  show_result("getf2_test()", res);

  res = gtdf2vfp_test();
  show_result("gtdf2vfp_test()", res);

  res = gtsf2vfp_test();
  show_result("gtsf2vfp_test()", res);

  res = gttf2_test();
  show_result("gttf2_test()", res);

  res = ledf2vfp_test();
  show_result("ledf2vfp_test()", res);

  res = lesf2vfp_test();
  show_result("lesf2vfp_test()", res);

  res = letf2_test();
  show_result("letf2_test()", res);

  res = lshrdi3_test();
  show_result("lshrdi3_test()", res);

  res = lshrti3_test();
  show_result("lshrti3_test()", res);

  res = ltdf2vfp_test();
  show_result("ltdf2vfp_test()", res);

  res = ltsf2vfp_test();
  show_result("ltsf2vfp_test()", res);

  res = lttf2_test();
  show_result("lttf2_test()", res);

  res = moddi3_test();
  show_result("moddi3_test()", res);

  res = modsi3_test();
  show_result("modsi3_test()", res);

  res = modti3_test();
  show_result("modti3_test()", res);

#ifdef HAS_COMPLEX
  res = muldc3_test();
  show_result("muldc3_test()", res);
#endif

  res = muldf3vfp_test();
  show_result("muldf3vfp_test()", res);

  res = muldi3_test();
  show_result("muldi3_test()", res);

  res = mulodi4_test();
  show_result("mulodi4_test()", res);

  res = mulosi4_test();
  show_result("mulosi4_test()", res);

  res = muloti4_test();
  show_result("muloti4_test()", res);

#ifdef HAS_COMPLEX
  res = mulsc3_test();
  show_result("mulsc3_test()", res);
#endif

  res = mulsf3vfp_test();
  show_result("mulsf3vfp_test()", res);

// no mulsi3.c
//  res = mulsi3_test();
//  show_result("mulsi3_test()", res);

#ifdef HAS_COMPLEX
  res = multc3_test();
  show_result("multc3_test()", res);
#endif

  res = multf3_test();
  show_result("multf3_test()", res);

  res = multi3_test();
  show_result("multi3_test()", res);

  res = mulvdi3_test();
  show_result("mulvdi3_test()", res);

  res = mulvsi3_test();
  show_result("mulvsi3_test()", res);

  res = mulvti3_test();
  show_result("mulvti3_test()", res);

#ifdef HAS_COMPLEX
  res = mulxc3_test();
  show_result("mulxc3_test()", res);
#endif

  return 0;
}

