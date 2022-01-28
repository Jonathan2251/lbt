void abort() {
  // cpu0.v: ABORT at mem 0x04
  asm("addiu $lr, $ZERO, 4");
  asm("ret $lr"); 
}
