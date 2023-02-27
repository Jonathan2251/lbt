//===- MyFrontendAction.h ---------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Example clang plugin which simply prints the names of all the top-level decls
// in the input file.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_EXAMPLE_MYFRONTENDACTION_H
#define LLVM_CLANG_EXAMPLE_MYFRONTENDACTION_H

#include "MyASTConsumer.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"

using namespace clang;

// MyFrontendAction is plugin, so after creating MyASTConsumer, the
// plugin Action's members will be destroyed. Therefore no C++'s pointer or
// reference should refer to Action.
class MyFrontendAction : public PluginASTAction {
protected:
  std::unique_ptr<ASTConsumer>
  CreateASTConsumer(CompilerInstance &CI, llvm::StringRef InFile) override;
  bool ParseArgs(const CompilerInstance &CI,
                 const std::vector<std::string> &args) override;
  virtual ActionType getActionType() override;
  bool usesPreprocessorOnly() const override;
};

#endif
