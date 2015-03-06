add_lld_library(lldCpu0elELFTarget
  Cpu0LinkingContext.cpp
  Cpu0TargetHandler.cpp
  Cpu0RelocationHandler.cpp
  Cpu0RelocationPass.cpp
  )

target_link_libraries(lldCpu0elELFTarget ${cmake_2_8_12_INTERFACE}
  lldCore
  )
