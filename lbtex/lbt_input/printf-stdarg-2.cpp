
/// start

#include "debug.h"
#include "print.h"

#define TEST_PRINTF

extern "C" int putchar(int c); 

extern "C" {
#include "printf-stdarg.c"
}
