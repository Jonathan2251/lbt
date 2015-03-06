
; /// start
; The @_start() exist to prevent lld linker error.
; Real entry (first instruction) is from cpu0BootAtomContent of 
; Cpu0RelocationPass.cpp jump to asm("start:") of start.cpp.
define void @_start() nounwind {
entry:
  ret void
}

define void @__start() nounwind {
entry:
  ret void
}

define void @__stack_chk_fail() nounwind {
entry:
  ret void
}

define void @__stack_chk_guard() nounwind {
entry:
  ret void
}

define void @_ZdlPv() nounwind {
entry:
  ret void
}

define void @__dso_handle() nounwind {
entry:
  ret void
}

define void @_ZNSt8ios_base4InitC1Ev() nounwind {
entry:
  ret void
}

define void @__cxa_atexit() nounwind {
entry:
  ret void
}

define void @_ZTVN10__cxxabiv120__si_class_type_infoE() nounwind {
entry:
  ret void
}

define void @_ZTVN10__cxxabiv117__class_type_infoE() nounwind {
entry:
  ret void
}

define void @_Znwm() nounwind {
entry:
  ret void
}

define void @__cxa_pure_virtual() nounwind {
entry:
  ret void
}

define void @_ZNSt8ios_base4InitD1Ev() nounwind {
entry:
  ret void
}

