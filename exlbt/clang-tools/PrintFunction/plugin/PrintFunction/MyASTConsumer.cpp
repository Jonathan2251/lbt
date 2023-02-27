//===- MyASTConsumer.cpp ---------------------------------------------===//
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
#include <fstream>
#include <regex>

using namespace clang;
using namespace std;

bool MyASTVisitor::VisitFunctionDecl(FunctionDecl *FD) {
  CurrFunc = FD;
  return true;
}

FunctionDecl *MyASTVisitor::getFunctionDecl(CallExpr *callExpr) {
  for (auto It = callExpr->child_begin();
       It != callExpr->child_end(); ++It) {
    if (auto Ice = dyn_cast_or_null<ImplicitCastExpr>(*It)) {
      for (auto It = Ice->child_begin();
           It != Ice->child_end(); ++It) {
        if (auto Dre = dyn_cast_or_null<DeclRefExpr>(*It)) {
          if (auto FD = dyn_cast_or_null<FunctionDecl>(
              Dre->getDecl())) {
            return FD;
          }
        }
      }
    }
  }
  return nullptr;
}

string MyASTVisitor::getArg0(CallExpr *ArgCallExpr) {
  Expr *E = ArgCallExpr->getArg(0);
  MY_DEBUG(
    llvm::dbgs() << "E: ";
    E->dump();
  );
  if (auto Uo = dyn_cast_or_null<UnaryOperator>(E)) {
    for (auto It = Uo->child_begin();
         It != Uo->child_end(); ++It) {
      if (auto Dre = dyn_cast_or_null<DeclRefExpr>(*It)) {
        MY_DEBUG(
          llvm::dbgs() << "Dre: ";
          Dre->dump();
        );
        return Dre->getNameInfo().getAsString();
      }
    }
  }
  return "";
}

int MyASTVisitor::getNumArg2(CallExpr *ArgCallExpr) {
  string Str = "";
  int N = 0;

  Expr *E = ArgCallExpr->getArg(3);
  MY_DEBUG(
    llvm::dbgs() << "E: ";
    E->dump();
  );
  if (auto Il = dyn_cast_or_null<IntegerLiteral>(E)) {
    llvm::APInt ApInt = Il->getValue();
    N = (int)ApInt.getSExtValue();
    return N;
  }
  return -1;
}

bool MyASTVisitor::getArrayOfArg2(CallExpr *ArgCallExpr, vector<string> &ArrayOfArg2, size_t N) {
  ArrayOfArg2.clear();
  Expr *Arg2 = ArgCallExpr->getArg(2);
  MY_DEBUG(llvm::dbgs() << "Arg2: ");
  MY_DEBUG(Arg2->dump());

  size_t i = 0;
  if (auto Ice = dyn_cast_or_null<ImplicitCastExpr>(Arg2)) {
    for (auto It = Ice->child_begin();
         It != Ice->child_end(); ++It) {
      if (auto Dre = dyn_cast_or_null<DeclRefExpr>(*It)) {
        MY_DEBUG(llvm::dbgs() << "ArrayOfArg2' name: " << Dre->getNameInfo().getAsString() << "\n");
        ValueDecl *ValD = Dre->getDecl();
        MY_DEBUG(
          string Str;
          llvm::raw_string_ostream S(Str);
          ValD->print(S);
          llvm::dbgs() << "S: " << S.str() << "\n";
        );
        if (auto VarD = dyn_cast_or_null<VarDecl>(ValD)) {
          if (auto Ile = dyn_cast_or_null<InitListExpr>(VarD->getInit())) {
            unsigned NInt = Ile->getNumInits();
            Expr **E = Ile->getInits();
            for (size_t i = 0; i < NInt; i++) {
              if (auto Cxxce = dyn_cast_or_null<CXXConstructExpr>(E[i])) {
                unsigned Na = Cxxce->getNumArgs();
                MY_DEBUG(llvm::dbgs() << "Na: " << Na << "\n");
                assert(Na == 1 && "Na != 1");
                if (Na != 1) llvm::dbgs() << "Na must == 1\n";
                Expr **E2 = Cxxce->getArgs();
                if (auto Ice = dyn_cast_or_null<ImplicitCastExpr>(E2[0])) {
                  for (auto It = Ice->child_begin();
                    It != Ice->child_end(); ++It) {
                    if (auto Dre = dyn_cast_or_null<DeclRefExpr>(*It)) {
                      ArrayOfArg2.push_back(Dre->getNameInfo().getAsString());
                    }
                  }
                }
              }
            }
          }
        }
        i++;
      }
    }
  }
  assert(ArrayOfArg2.size() == N && "ArrayOfArg2.size() != N");
  return true;
}

bool MyASTVisitor::VisitStmt(Stmt *X) {
  if (!CurrFunc) {
    return true;
  }
  MY_DEBUG(llvm::dbgs() << "CurrFunc name: " << CurrFunc->getQualifiedNameAsString() 
                 << "\n");
  if (auto Ce = dyn_cast_or_null<CallExpr>(X)) {
    FunctionDecl *FD = getFunctionDecl(Ce);
    if (FD->getQualifiedNameAsString() == "GetFuncAndArgs") {
      string Arg0 = getArg0(Ce);
      int N = getNumArg2(Ce);
      assert(N != -1 && "N == -1");
      vector<string> ArrayOfArg2;
      bool Res = getArrayOfArg2(Ce, ArrayOfArg2, (size_t)N);
      MY_DEBUG(llvm::dbgs() << "Res: " << Res << "\n");
      assert(Res > 0 && "Res <= 0");
      if (Res <= 0) llvm::dbgs() << "Res must > 0\n";

      llvm::dbgs() << "Function Name: " << FD->getQualifiedNameAsString() << "\n";
      llvm::dbgs() << "Arg0: " << Arg0 << "\n";

      auto print = [](const string& N) { llvm::dbgs() << N << ", "; };
      llvm::dbgs() << "ArrayOfArg2: size: " << ArrayOfArg2.size() << ", value: ";
      for_each(ArrayOfArg2.cbegin(), ArrayOfArg2.cend(), print);
      llvm::dbgs() << "\n";
    }
  }
  return true;
}

MyASTConsumer::MyASTConsumer(CompilerInstance &CI)
    : Visitor(&CI.getASTContext()) {
  llvm::dbgs() << "** Creating MyASTConsumer: " << "\n";
}

// Override the method that gets called for each parsed top-level
// declaration.
bool MyASTConsumer::HandleTopLevelDecl(DeclGroupRef DR) {
  for (DeclGroupRef::iterator B = DR.begin(), E = DR.end(); B != E; ++B) {
    // Traverse the declaration using our AST visitor.
    Visitor.TraverseDecl(*B);
  }
  return true;
}

void MyASTConsumer::HandleTranslationUnit(clang::ASTContext &Context) {

  // CodeGen.outputFile();
}

bool MyASTConsumer::TraverseDecl(Decl *D) {
  return Visitor.TraverseDecl(D);
}
