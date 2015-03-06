char* strcpy(char * dest, const char * src) {
  char* p;

  for (p = dest; *src != '\0'; src++, p++)
    *p = *src;

  return dest;
}

