#ifndef _DYNAMIC_LINKER_H_
#define _DYNAMIC_LINKER_H_

#define DYNLINKER_INFO_ADDR  0x70000
#define DYNENT_SIZE          4
#define DYNPROGSTART         0x40000
#define FLASHADDR            0xA0000
#define GPADDR               0x7FFF0

#include "debug.h"

struct ProgAddr {
  int memAddr;
  int size;
};

extern void dynamic_linker_init();
extern void dynamic_linker();

#endif
