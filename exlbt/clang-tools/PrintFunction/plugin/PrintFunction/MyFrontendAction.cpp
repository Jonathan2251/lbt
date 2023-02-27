//===- MyFrontendAction.cpp ---------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Example clang plugin which do convert from cpp to cpp.
//
//===----------------------------------------------------------------------===//

#include "MyFrontendAction.h"

using namespace clang;


std::unique_ptr<ASTConsumer>
MyFrontendAction::CreateASTConsumer(CompilerInstance &CI,
                            llvm::StringRef InFile) {
  return std::make_unique<MyASTConsumer>(CI);
}

bool MyFrontendAction::ParseArgs(const CompilerInstance &CI,
                         const std::vector<std::string> &args) {
  return true;
}

// http://clang-developers.42468.n3.nabble.com/AST-modifications-that-apply-to-the-binary-td4054289.html
MyFrontendAction::ActionType MyFrontendAction::getActionType() {
  return AddBeforeMainAction;
}

bool MyFrontendAction::usesPreprocessorOnly() const { return false; }

static FrontendPluginRegistry::Add<MyFrontendAction>
    X("ex1-act", "convert cpp to cpp");
