extern int VSNPrintf(char *buff, int buff_length,
              const char *format, va_list args);

static int prints(const char *string)
{
  register int pc = 0, padchar = ' ';

  for ( ; *string ; ++string) {
    putchar (*string);
    ++pc;
  }

  return pc;
}

int printf(const char *format, ...) {
  int length = 1000;
  char buffer[1000];
  va_list args;
  va_start(args, format);
  int needed_length = VSNPrintf(buffer, length, format, args);
  va_end(args);
  prints(buffer);
}
