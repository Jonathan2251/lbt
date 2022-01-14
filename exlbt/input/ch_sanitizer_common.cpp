#include <stdarg.h>
#include "sanitizer_file.h"

extern "C" int putchar(int c);

namespace __sanitizer {
  extern int VSNPrintf(char *buff, int buff_length,
                const char *format, va_list args);

  bool WriteToFile(fd_t fd, const void *buff, uptr buff_size, uptr *bytes_written,
                   error_t *error_p) {
    return true;
  }
}

int prints(const char *string)
{
  register int pc = 0, padchar = ' ';

  for ( ; *string ; ++string) {
    putchar (*string);
    ++pc;
  }

  return pc;
}

extern "C" int printf(const char *format, ...) {
  int length = 1000;
  char buffer[1000];
  va_list args;
  va_start(args, format);
  int needed_length = __sanitizer::VSNPrintf(buffer, length, format, args);
  va_end(args);
  prints(buffer);
  return 0;
}
