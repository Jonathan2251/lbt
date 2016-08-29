# elf2hex.cpp needs backend related functions, like 
# LLVMInitializeCpu0TargetInfo and LLVMInitializeCpu0Disassembler ... etc.
# Set LLVM_LINK_COMPONENTS to ${LLVM_TARGETS_TO_BUILD} then it can link them
# during the link stage.
set(LLVM_LINK_COMPONENTS
  ${LLVM_TARGETS_TO_BUILD}
  )

add_llvm_tool(elf2hex
  elf2hex.cpp
  elf2hex-dlinker.cpp
  )
