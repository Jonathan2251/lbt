
/// start

#include "print.cpp"

#include "ch4_1_1.cpp"
#include "ch4_3.cpp"
#include "ch4_5.cpp"
#include "ch7_1.cpp"
#include "ch7_2_2.cpp"
#include "ch7_3.cpp"
#include "ch7_4.cpp"
#include "ch8_1_1.cpp"
#include "ch8_2.cpp"
#include "ch9_1_4.cpp"
#include "ch9_3.cpp"

int test_nolld()
{
  bool pass = true;
  int a = 0;
  asm("addiu	$4, $zero, 4"); // 0x0f
  asm("addiu	$5, $zero, 30"); //
  asm("jsub	rot0"); // rolv $2, $4, %5
  asm("nop");
  __asm__ __volatile__("addiu %0,$2,0"
                       :"=r"(a)
                       );
  print_integer(a);  // a = 0x1 = 1
  asm("addiu	$4, $zero, 8");
  asm("jsub	rot1"); // rol $2, $4, 30
  asm("nop");
  __asm__ __volatile__("addiu %0,$2,0"
                       :"=r"(a)
                       );
  print_integer(a);  // a = 2
  asm("addiu	$4, $zero, 1");
  asm("addiu	$5, $zero, 30"); //
  asm("jsub	rot2"); // rorv $2, $4, $5
  asm("nop");
  __asm__ __volatile__("addiu %0,$2,0"
                       :"=r"(a)
                       );
  print_integer(a);  // a = 4
  asm("addiu	$4, $zero, 1");
  asm("addiu	$5, $zero, 3");
  asm("jsub	rot0"); // rolv $2, $4, %5
  asm("nop");
  __asm__ __volatile__("addiu %0,$2,0"
                       :"=r"(a)
                       );
  print_integer(a);  // a = 8
  a = test_math();
  print_integer(a);  // a = 0x
  if (a != 74) pass = false;
  a = test_div();
  print_integer(a);  // a = 253
  if (a != 253) pass = false;
  a = test_local_pointer();
  print_integer(a);  // a = 3
  if (a != 3) pass = false;
  a = (int)test_load_bool();
  print_integer(a);  // a = 1
  if (a != 1) pass = false;
  a = test_andorxornot();
  print_integer(a); // a = 14
  if (a != 14) pass = false;
  a = test_setxx();
  print_integer(a); // a = 3
  if (a != 3) pass = false;
  a = test_signed_char();
  print_integer(a); // a = -126
  if (a != -126) pass = false;
  a = test_unsigned_char();
  print_integer(a); // a = 130
  if (a != 130) pass = false;
  a = test_signed_short();
  print_integer(a); // a = -32766
  if (a != -32766) pass = false;
  a = test_unsigned_short();
  print_integer(a); // a = 32770
  if (a != 32770) pass = false;
  long long b = test_longlong();
  print_integer((int)(b >> 32)); // 393307
  if ((int)(b >> 32) != 393307) pass = false;
  print_integer((int)b); // 16777222
  if ((int)(b) != 16777222) pass = false;
  a = test_control1();
  print_integer(a);	// a = 51
  if (a != 51) pass = false;
  a = test_DelUselessJMP();
  print_integer(a); // a = 2
  if (a != 2) pass = false;
  print_integer(2147483647); // test mod % (mult) from itoa.cpp
  print_integer(-2147483648); // test mod % (multu) from itoa.cpp
  a = test_madd();
  print_integer(a); // a = 7
  if (a != 7) pass = false;
  a = test_vararg();
  print_integer(a); // a = 15
  if (a != 15) pass = false;

  return pass;
}

/* result:
74
253
3
1
14
3
-126
130
-32766
32770
393307
16777222
51
2
289
2147483647
-2147483648
7
15
RET to PC < 0, finished!
*/
