
/// start

#include "debug.h"
#include "print.h"

#define PRINT_TEST

extern "C" int printf(const char *format, ...);
extern "C" int sprintf(char *out, const char *format, ...);

extern unsigned char sBuffer[4];
extern int test_overflow();
extern int test_add_overflow();
extern int test_sub_overflow();
extern int test_ctrl2();
extern int select_1();
extern int select_2();
extern int select_3();
extern int test_select_global_pic();
extern int test_tailcall(int a);
extern int test_alloc();

extern int test_staticlink();
