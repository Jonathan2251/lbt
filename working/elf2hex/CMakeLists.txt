# elf2hex.cpp needs backend related functions, like 
# LLVMInitializeCpu0TargetInfo and LLVMInitializeCpu0Disassembler ... etc.
# Set LLVM_LINK_COMPONENTS then it can link them during the link stage.
set(LLVM_LINK_COMPONENTS
  AllTargetsAsmPrinters
  AllTargetsDescs
  AllTargetsDisassemblers
  AllTargetsInfos
  CodeGen
  MC
  MCDisassembler
  Object
  Support
  )

add_llvm_tool(elf2hex
  elf2hex.cpp
  )

if(HAVE_LIBXAR)
  target_link_libraries(elf2hex ${XAR_LIB})
endif()
