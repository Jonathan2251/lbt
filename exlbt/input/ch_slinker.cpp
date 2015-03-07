// bash build-run_backend.sh

/// start

//#define TEST_SELECT

#include "ch_nolld.h"
#include "ch_lld_staticlink.h"

int main()
{
  bool pass = true;
  pass = test_nolld();
  if (pass)
    printf("test_nolld(): PASS\n");
  else
    printf("test_nolld(): FAIL\n");
  pass = true;
  pass = test_staticlink();

  return pass;
}

#include "ch_nolld.cpp"
#include "ch_lld_staticlink.cpp"
