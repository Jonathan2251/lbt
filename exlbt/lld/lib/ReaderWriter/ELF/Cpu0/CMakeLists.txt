add_llvm_library(lldCpu0ELFTarget
  Cpu0ExecutableWriter.cpp
  Cpu0LinkingContext.cpp
  Cpu0TargetHandler.cpp
  Cpu0RelocationHandler.cpp
  Cpu0RelocationPass.cpp
  Cpu0SectionChunks.cpp
  LINK_LIBS
    lldELF
    lldReaderWriter
    lldCore
    LLVMObject
    LLVMSupport
  )

include_directories(.)
