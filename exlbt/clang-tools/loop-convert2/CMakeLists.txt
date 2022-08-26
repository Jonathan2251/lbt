set(LLVM_LINK_COMPONENTS support)

add_clang_executable(loop-convert2
  LoopConvert2.cpp
  )
target_link_libraries(loop-convert2
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangSerialization
  clangTooling
  )
