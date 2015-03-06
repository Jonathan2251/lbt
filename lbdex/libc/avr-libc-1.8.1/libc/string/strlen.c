#define size_t unsigned int

size_t strlen (const char * str) {
  size_t n;

  for (n = 0; str[n] != '\0'; n++);

  return n;
}

