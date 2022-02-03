
/// start

#include "start.h"

extern int main();

// Real entry (first instruction) is from cpu0BootAtomContent of 
// Cpu0RelocationPass.cpp jump to asm("start:") of start.cpp.
void start() {
  asm("start:");
  
  INIT_SP
  initRegs();
  main();
  asm("addiu $lr, $ZERO, -1");
  asm("ret $lr");
}

