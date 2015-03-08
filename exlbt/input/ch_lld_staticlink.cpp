
/// start

#include "ch6_1.cpp"
#include "ch9_2_1.cpp"
#include "ch9_2_2.cpp"
#include "ch9_3_2.cpp"
#include "ch11_2.cpp"
#include "ch12_inherit.cpp"

int verify_test_ctrl2()
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

  return 0;
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
  a = verify_test_ctrl2();
  a = select_1();
  printf("select_1() = %d\n", a); // a = 1
  a = select_2();
  printf("select_2() = %d\n", a); // a = 1
  a = select_3();
  printf("select_3() = %d\n", a); // a = 1
  a = test_select_global_pic(); // test global of pic llc -O1 option
  printf("test_select_global_pic() = %d", a); // a = 100
  if (a == 100)
    printf(", PASS\n");
  else
    printf(", FAIL\n");
  a = test_tailcall(5);
  printf("test_tailcall(5) = %d", a); // a = 120
  if (a == 120)
    printf(", PASS\n");
  else
    printf(", FAIL\n");
  a = test_func_arg_struct();
  a = test_contructor();
  a = test_template();
  printf("test_template() = %d", a); // a = 15
  if (a == 15)
    printf(", PASS\n");
  else
    printf(", FAIL\n");
  a = test_alloc();  // 31
  printf("test_alloc() = %d", a);
  if (a == 31)
    printf(", PASS\n");
  else
    printf(", FAIL\n");
  a = test_inlineasm();
  printf("test_inlineasm() = %d", a); // a = 53
  if (a == 53)
    printf(", PASS\n");
  else
    printf(", FAIL\n");
  a = test_cpp_polymorphism();
  printf("test_cpp_polymorphism() = %d", a); // a = 0
  if (a == 0)
    printf(", PASS\n");
  else
    printf(", FAIL\n");
  test_build();

  int_sim();

  return 0;
}
