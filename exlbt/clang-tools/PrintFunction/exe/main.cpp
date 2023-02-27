#include <sstream>
#include <string>
#include "clang/Driver/Driver.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/Error.h"
#include "../plugin/PrintFunction/MyASTConsumer.h"
#include "../plugin/PrintFunction/MyASTConsumer.cpp"

using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;

static llvm::cl::OptionCategory ToolingSampleCategory("Tooling Sample");

// For each source file provided to the tool, a new FrontendAction is created.
class MyFrontendAction : public ASTFrontendAction {
public:
  MyFrontendAction() {}
  void EndSourceFileAction() override {
  }

  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 StringRef file) override {
    llvm::errs() << "** Creating AST consumer for: " << file << "\n";
    TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
    return std::make_unique<MyASTConsumer>(CI);
  }

private:
  Rewriter TheRewriter;
};

int main(int argc, const char **argv) {
  llvm::Expected<clang::tooling::CommonOptionsParser> op =
  clang::tooling::CommonOptionsParser::create(argc, argv, ToolingSampleCategory, llvm::cl::OneOrMore);
  assert(static_cast<bool>(op) && "FAIL at static_cast<bool>(op)");  // Should not return an error.
  ClangTool Tool(op->getCompilations(), op->getSourcePathList());

  // ClangTool::run accepts a FrontendActionFactory, which is then used to
  // create new objects implementing the FrontendAction interface. Here we use
  // the helper newFrontendActionFactory to create a default factory that will
  // return a new MyFrontendAction object every time.
  // To further customize this, we could create our own factory class.
  return Tool.run(newFrontendActionFactory<MyFrontendAction>().get());
}

