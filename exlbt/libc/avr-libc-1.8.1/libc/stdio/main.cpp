
extern "C" int printf(const char *format, ...);

int main()
{
  bool pass = true;
  if (pass)
    printf("test_nolld(): PASS\n");
  else
    printf("test_nolld(): FAIL\n");

  return pass;
}

