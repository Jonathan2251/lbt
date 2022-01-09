
/// start

#include "print.h"

// Definition putchar(int c) for printf-stdarg.c
// For memory IO
int putchar(int c)
{
  char *p = (char*)IOADDR;
  *p = c;

  return 0;
}
