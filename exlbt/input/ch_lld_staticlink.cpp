
/// start

#include "ch4_1_addsuboverflow.cpp"
#include "ch8_1_br_jt.cpp"
#include "ch8_2_phinode.cpp"
#include "ch8_1_blockaddr.cpp"
#include "ch8_2_longbranch.cpp"
#include "ch9_2_tailcall.cpp"
#include "ch9_3_detect_exception.cpp"

void verify_test_ctrl2()
{
  int a = -1;
  int b = -1;
  int c = -1;
  int d = -1;

  sBuffer[0] = (unsigned char)0x35;
  sBuffer[1] = (unsigned char)0x35;
  a = test_ctrl2();
  sBuffer[0] = (unsigned char)0x30;
  sBuffer[1] = (unsigned char)0x29;
  b = test_ctrl2();
  sBuffer[0] = (unsigned char)0x35;
  sBuffer[1] = (unsigned char)0x35;
  c = test_ctrl2();
  sBuffer[0] = (unsigned char)0x34;
  d = test_ctrl2();
  printf("test_ctrl2(): a = %d, b = %d, c = %d, d = %d", a, b, c, d);
  if (a == 1 && b == 0 && c == 1 && d == 0)
    printf(", PASS\n");
  else
    printf(", FAIL\n");

  return;
}

int test_staticlink()
{
  int a = 0;

  a = test_add_overflow();
  a = test_sub_overflow();
  a = test_global();  // gI = 100
  printf("global variable gI = %d", a);
  if (a == 100)
    printf(", PASS\n");
  else
    printf(", FAIL\n");
  verify_test_ctrl2();
  a = test_phinode(3, 1, 0);
  printf("test_phinode(3, 1) = %d", a); // a = 3
  if (a == 3)
    printf(", PASS\n");
  else
    printf(", FAIL\n");
  a = test_blockaddress(1);
  printf("test_blockaddress(1) = %d", a); // a = 1
  if (a == 1)
    printf(", PASS\n");
  else
    printf(", FAIL\n");
  a = test_blockaddress(2);
  printf("test_blockaddress(2) = %d", a); // a = 2
  if (a == 2)
    printf(", PASS\n");
  else
    printf(", FAIL\n");
  a = test_longbranch();
  printf("test_longbranch() = %d", a); // a = 0
  if (a == 0)
    printf(", PASS\n");
  else
    printf(", FAIL\n");
  a = test_func_arg_struct();
  printf("test_func_arg_struct() = %d", a); // a = 0
  if (a == 0)
    printf(", PASS\n");
  else
    printf(", FAIL\n");
  a = test_constructor();
  printf("test_constructor() = %d", a); // a = 0
  if (a == 0)
    printf(", PASS\n");
  else
    printf(", FAIL\n");
  a = test_template();
  printf("test_template() = %d", a); // a = 15
  if (a == 15)
    printf(", PASS\n");
  else
    printf(", FAIL\n");
  long long res = test_template_ll();
  printf("test_template_ll() = 0x%X-%X", (int)(res>>32), (int)res); // res = -1
  if (res == -1)
    printf(", PASS\n");
  else
    printf(", FAIL\n");
  a = test_tailcall(5);
  printf("test_tailcall(5) = %d", a); // a = 15
  if (a == 120)
    printf(", PASS\n");
  else
    printf(", FAIL\n");
  test_detect_exception(true);
  printf("exceptionOccur= %d", exceptionOccur);
  if (exceptionOccur)
    printf(", PASS\n");
  else
    printf(", FAIL\n");
  test_detect_exception(false);
  printf("exceptionOccur= %d", exceptionOccur);
  if (!exceptionOccur)
    printf(", PASS\n");
  else
    printf(", FAIL\n");
  a = inlineasm_global(); // 4
  printf("inlineasm_global() = %d", a); // a = 4
  if (a == 4)
    printf(", PASS\n");
  else
    printf(", FAIL\n");
  a = test_cpp_polymorphism();
  printf("test_cpp_polymorphism() = %d", a); // a = 0
  if (a == 0)
    printf(", PASS\n");
  else
    printf(", FAIL\n");

  int_sim();

  return 0;
}

// test passing compilation only
#include "builtins-cpu0.c"
