// https://static.linaro.org/connect/yvr18/presentations/yvr18-223.pdf

// Declares clang::SyntaxOnlyAction.
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
// Declares llvm::cl::extrahelp.
#include "llvm/Support/CommandLine.h"
#include "clang/AST/ASTContext.h"

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

#include "clang/Frontend/CompilerInstance.h"
#include "clang/Rewrite/Core/Rewriter.h"
#define DEBUG_TYPE "LoopConvert2"
#include "llvm/Support/Debug.h"
#include <iostream>

using namespace clang;
using namespace clang::ast_matchers;

using namespace clang::tooling;
using namespace llvm;

// Apply a custom category to all command-line options so that they are the
// only ones displayed.
static llvm::cl::OptionCategory MyToolCategory("my-tool options");

// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

// A help message for this specific tool can be added afterwards.
static cl::extrahelp MoreHelp("\nMore help text...\n");

StatementMatcher MyMatcher =
            callExpr(callee(functionDecl(hasName("GetFuncAndArgs")))/*.hasArgument(0).bind("node")*//*,
  argumentCountIs(5)*/
  ).bind("callFunc");

static bool areSameVariable(const ValueDecl *First, const ValueDecl *Second) {
  return First && Second &&
         First->getCanonicalDecl() == Second->getCanonicalDecl();
}


class MyPrinter : public MatchFinder::MatchCallback {
public :
  virtual void run(const MatchFinder::MatchResult &Result) {
    const ASTContext *Context = Result.Context;
    const CallExpr *CE = Result.Nodes.getNodeAs<CallExpr>("callFunc");
    // We do not want to convert header files!
#if 0
    if (!CE || !Context->getSourceManager().isWrittenInMainFile(CE->getForLoc()))
      return;
#endif
#if 0
    const VarDecl *IncVar = Result.Nodes.getNodeAs<VarDecl>("incVarName");
    const VarDecl *CondVar = Result.Nodes.getNodeAs<VarDecl>("condVarName");
    const VarDecl *InitVar = Result.Nodes.getNodeAs<VarDecl>("initVarName");

    if (!areSameVariable(IncVar, CondVar) || !areSameVariable(IncVar, InitVar))
      return;
    llvm::outs() << "Potential array-based loop discovered.\n";
#endif
    if (const CallExpr *CE = Result.Nodes.getNodeAs<clang::CallExpr>("callFunc")) {
      LLVM_DEBUG(CE->dump());

      // Display Row and Column for begin and end of ForStmt
      SourceRange SR = CE->getSourceRange();
      FullSourceLoc FSBeginLoc = Context->getFullLoc(SR.getBegin());
      assert(FSBeginLoc.isValid());
      llvm::dbgs() << "CallExpr begin at "
                   << FSBeginLoc.getSpellingLineNumber() << ":"
                   << FSBeginLoc.getSpellingColumnNumber() << "\n";
      FullSourceLoc FSEndLoc = Context->getFullLoc(SR.getEnd());
      assert(FSEndLoc.isValid());
      llvm::dbgs() << "CallExpr end at "
                   << FSEndLoc.getSpellingLineNumber() << ":"
                   << FSEndLoc.getSpellingColumnNumber() << "\n";
    }
  }

public:
  Rewriter *TheRewriter;
};

int main(int argc, const char **argv) {
  auto ExpectedParser = CommonOptionsParser::create(argc, argv, MyToolCategory);
  if (!ExpectedParser) {
    // Fail gracefully for unsupported options.
    llvm::errs() << ExpectedParser.takeError();
    return 1;
  }
  CommonOptionsParser& OptionsParser = ExpectedParser.get();
  ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());

  MyPrinter Printer;
  MatchFinder Finder;
  Finder.addMatcher(MyMatcher, &Printer);
  
  //Printer.TheRewriter = &MFA->TheRewriter;

  std::unique_ptr<FrontendActionFactory> pFAF = newFrontendActionFactory(&Finder);

  return Tool.run(pFAF.get());
}

