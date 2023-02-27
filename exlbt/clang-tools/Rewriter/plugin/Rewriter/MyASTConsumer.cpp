//===- CppDslEx1.cpp ---------------------------------------------===//
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

#include "MyASTConsumer.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Sema/Sema.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;

MyCodeGen::MyCodeGen(CompilerInstance &CI) {
  TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
}

void MyCodeGen::createBuiltinFunc(CallExpr *callExpr,
                                           ASTContext *Context,
                                           FunctionDecl *CurrFunc,
                                           FunctionDecl *FD) {
  llvm::dbgs() << "Function Name: " << FD->getQualifiedNameAsString() << "\n";
  FullSourceLoc FDBeginLoc = Context->getFullLoc(FD->getBeginLoc());
  assert(FDBeginLoc.isValid());
  llvm::dbgs() << "Found Function Declaration begin at "
               << FDBeginLoc.getSpellingLineNumber() << ":"
               << FDBeginLoc.getSpellingColumnNumber() << "\n";
  FullSourceLoc FDEndLoc = Context->getFullLoc(FD->getEndLoc());
  assert(FDEndLoc.isValid());
  llvm::dbgs() << "Found Function Declaration end at "
               << FDEndLoc.getSpellingLineNumber() << ":"
               << FDEndLoc.getSpellingColumnNumber() << "\n";

  auto CallerFuncName = *callExpr->child_begin();
  SourceRange CallerSR = CallerFuncName->getSourceRange();
  FullSourceLoc CallerBeginLoc = Context->getFullLoc(CallerSR.getBegin());
  assert(CallerBeginLoc.isValid());
  llvm::dbgs() << "Found Caller begin at "
               << CallerBeginLoc.getSpellingLineNumber() << ":"
               << CallerBeginLoc.getSpellingColumnNumber() << "\n";
  FullSourceLoc CallerEndLoc = Context->getFullLoc(CallerSR.getEnd());
  assert(CallerEndLoc.isValid());
  llvm::dbgs() << "Found Caller end at "
               << CallerEndLoc.getSpellingLineNumber() << ":"
               << CallerEndLoc.getSpellingColumnNumber() << "\n";

  if (FD->getQualifiedNameAsString() == "A::abs") {
    // Use getSourceRange() instead of getBeginLoc() since A:: and abc may sit at
    // different lines.
    char sBuiltinFunc[] = "__builtin_fabs";
    TheRewriter.ReplaceText(CallerSR, StringRef(sBuiltinFunc));
  }
}

void MyCodeGen::outputFile() {
  SourceManager &SM = TheRewriter.getSourceMgr();
  StringRef inFileName = SM.getFileEntryForID(SM.getMainFileID())->getName();
  std::string outFileName = "output.cpp";
  llvm::dbgs() << "input file: " << inFileName << "\n";
  llvm::dbgs() << "output file: " << outFileName << "\n";

  // Now emit the rewritten buffer.
  std::error_code EC;
  llvm::raw_fd_ostream OS(outFileName, EC);
  TheRewriter.getEditBuffer(SM.getMainFileID()).write(OS);
  // TheRewriter.overwriteChangedFiles();
}

bool MyASTVisitor::VisitFunctionDecl(FunctionDecl *FD) {
  CurrFunc = FD;
  return true;
}

FunctionDecl *MyASTVisitor::GetFunctionDecl(CallExpr *callExpr) {
  for (auto cait = callExpr->child_begin();
       cait != callExpr->child_end(); ++cait) {
    if (auto impexpr = dyn_cast_or_null<ImplicitCastExpr>(*cait)) {
      for (auto imit = impexpr->child_begin();
           imit != impexpr->child_end(); ++imit) {
        if (auto decl_ref = dyn_cast_or_null<DeclRefExpr>(*imit)) {
          if (auto FD = dyn_cast_or_null<FunctionDecl>(
              decl_ref->getDecl())) {
            return FD;
          }
        }
      }
    }
  }
  return nullptr;
}

bool MyASTVisitor::VisitStmt(Stmt *x) {
  if (!CurrFunc) {
    return true;
  }
  // Use _Noreturn as an example for adding keyword before function.
  // Search "_Noreturn" in clang/include clang/lib to see how to add keyword
  //   for function.
  llvm::dbgs() << "CurrFunc name: " << CurrFunc->getQualifiedNameAsString()
               << ", CurrFunc->hasAttr<C11NoReturnAttr>(): "
               << CurrFunc->hasAttr<C11NoReturnAttr>() << "\n";
  if (!CurrFunc->hasAttr<C11NoReturnAttr>()) {
    return true;
  }
  // double result = A::abs(x); or result = A::abs(x) or return A::abs(x), ...
  if (auto callExpr = dyn_cast_or_null<CallExpr>(x)) {
    FunctionDecl *FD = GetFunctionDecl(callExpr);
    if (FD->getQualifiedNameAsString() == "A::abs") {
      CodeGen.createBuiltinFunc(callExpr, Context, CurrFunc, FD);
    }
  }
  return true;
}

MyASTConsumer::MyASTConsumer(CompilerInstance &CI)
    : CodeGen(CI), Visitor(&CI.getASTContext(), CodeGen) {
  llvm::dbgs() << "** Creating MyASTConsumer: " << "\n";
}

// Override the method that gets called for each parsed top-level
// declaration.
bool MyASTConsumer::HandleTopLevelDecl(DeclGroupRef DR) {
  for (DeclGroupRef::iterator b = DR.begin(), e = DR.end(); b != e; ++b) {
    // Traverse the declaration using our AST visitor.
    Visitor.TraverseDecl(*b);
  }
  return true;
}

void MyASTConsumer::HandleTranslationUnit(clang::ASTContext &Context) {

  CodeGen.outputFile();
}

bool MyASTConsumer::TraverseDecl(Decl *D) {
  return Visitor.TraverseDecl(D);
}
