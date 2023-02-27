set(LLVM_LINK_COMPONENTS support)

add_clang_executable(PrintFunction
  PrintFunction.cpp
  )
target_link_libraries(PrintFunction
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangSerialization
  clangTooling
  )
