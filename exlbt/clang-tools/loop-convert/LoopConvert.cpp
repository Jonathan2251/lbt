// https://static.linaro.org/connect/yvr18/presentations/yvr18-223.pdf

// Declares clang::SyntaxOnlyAction.
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
// Declares llvm::cl::extrahelp.
#include "llvm/Support/CommandLine.h"

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

#include "clang/Frontend/CompilerInstance.h"
#include "clang/Rewrite/Core/Rewriter.h"
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

StatementMatcher LoopMatcher =
  forStmt(hasLoopInit(declStmt(hasSingleDecl(varDecl(
    hasInitializer(integerLiteral(equals(0)))))))).bind("forLoop");

class LoopPrinter : public MatchFinder::MatchCallback {
public :
  virtual void run(const MatchFinder::MatchResult &Result) {
    const ASTContext *Context = Result.Context;
    if (const ForStmt *FS = Result.Nodes.getNodeAs<clang::ForStmt>("forLoop")) {
      FS->dump();

      // Display Row and Column for begin and end of ForStmt
      SourceRange SR = FS->getSourceRange();
      FullSourceLoc FSBeginLoc = Context->getFullLoc(SR.getBegin());
      assert(FSBeginLoc.isValid());
      llvm::dbgs() << "ForStmt begin at "
                   << FSBeginLoc.getSpellingLineNumber() << ":"
                   << FSBeginLoc.getSpellingColumnNumber() << "\n";
      FullSourceLoc FSEndLoc = Context->getFullLoc(SR.getEnd());
      assert(FSEndLoc.isValid());
      llvm::dbgs() << "ForStmt end at "
                   << FSEndLoc.getSpellingLineNumber() << ":"
                   << FSEndLoc.getSpellingColumnNumber() << "\n";
    }
  }

public:
  Rewriter *TheRewriter;
};

/*
// For each source file provided to the tool, a new FrontendAction is created.
class MyFrontendAction : public ASTFrontendAction {
public:
  MyFrontendAction() {}
  void EndSourceFileAction() override {
    SourceManager &SM = TheRewriter.getSourceMgr();
    llvm::errs() << "** EndSourceFileAction for: "
                 << SM.getFileEntryForID(SM.getMainFileID())->getName() << "\n";

    // Now emit the rewritten buffer.
    TheRewriter.getEditBuffer(SM.getMainFileID()).write(llvm::outs());
  }

  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 StringRef file) override {
    // set Rewriter
    llvm::errs() << "** Creating AST consumer for: " << file << "\n";
    TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
    return std::make_unique<ASTConsumer>(CI);
  }

public:
  Rewriter TheRewriter;
};*/


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
#if 0
  return Tool.run(newFrontendActionFactory<clang::SyntaxOnlyAction>().get());
#else

/*  MyFrontendAction FA;
  std::unique_ptr<FrontendActionFactory> MFA = newFrontendActionFactory<MyFrontendAction>(&FA);
  Tool.run(MFA.get());*/

  LoopPrinter Printer;
  MatchFinder Finder;
  Finder.addMatcher(LoopMatcher, &Printer);
  
  //Printer.TheRewriter = &MFA->TheRewriter;

  std::unique_ptr<FrontendActionFactory> pFAF = newFrontendActionFactory(&Finder);

  return Tool.run(pFAF.get());
#endif
}

