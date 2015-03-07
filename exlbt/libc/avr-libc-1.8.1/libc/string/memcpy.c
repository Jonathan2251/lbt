#define size_t unsigned int

void * memcpy ( void * dest, const void * src, size_t num ) {
  const char* s = src;
  char* p;

  for (p = (char*)dest; num > 0; s++, p++, num--)
    *p = *s;

  return (void*)0;
}

