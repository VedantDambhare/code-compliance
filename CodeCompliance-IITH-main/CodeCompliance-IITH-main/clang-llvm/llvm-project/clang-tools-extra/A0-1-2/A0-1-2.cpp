#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include <iostream>
#include "clang/Tooling/Inclusions/HeaderIncludes.h"
#include "clang/Tooling/Core/Replacement.h"


#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Lex/PreprocessorOptions.h"
#include "clang/Lex/HeaderSearch.h"
#include "clang/Basic/FileManager.h"





using namespace std;
using namespace clang;
using namespace clang::tooling;
using namespace clang::ast_matchers;
using namespace llvm;



// Apply a custom category to all command-line options so that they are the
// only ones displayed.
static llvm::cl::OptionCategory simple_integer_types("simple_integer_types options");

// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

// A help message for this specific tool can be added afterwards.
static cl::extrahelp MoreHelp("\nMore help text...\n");

// Define a matcher for switch case statements.
// StatementMatcher LabelStmtMatcher = labelStmt().bind("LabelStmt");
// StatementMatcher GotoStmtMatcher = gotoStmt().bind("GotoStmt");
// DeclarationMatcher FunctionDeclMatcher = functionDecl().bind("function");
// Matcher to find all header includes.
// StatementMatcher callExprDeclaration = callExpr(callee(functionDecl(unless(isNoReturn())).bind('declaration'))).bind("expr");
// callee( functionDecl(unless(isNoReturn())))
StatementMatcher callExprDeclaration = callExpr(callee(functionDecl(unless(isNoReturn()))), unless(hasAncestor(varDecl())),unless(hasAncestor(ifStmt())),unless(hasAncestor(forStmt())),unless(hasAncestor(whileStmt())),unless(hasAncestor(returnStmt())),unless(hasAncestor(cxxStaticCastExpr()))).bind("function_call");

class Simple_Pass_Handler : public MatchFinder::MatchCallback {
public:
 
  virtual void run(const MatchFinder::MatchResult &Result) override {

    
    if (const clang::CallExpr *expre = Result.Nodes.getNodeAs<clang::CallExpr>("function_call")){

      const clang::FunctionDecl *FD = expre -> getDirectCallee();
      clang::QualType Qual = FD -> getReturnType();
      
      if ( (Qual.getTypePtr() -> isVoidType()) == false ) {
        DiagnosticsEngine &Diagnostics = Result.Context->getDiagnostics();
        unsigned DiagID = Diagnostics.getCustomDiagID(DiagnosticsEngine::Error, " AUTOSAR C++ (Rule A0-1-2): Violated -The value returned by a function %0 having a non-void return type that is not an overloaded operator shall be used.");
        Diagnostics.Report(expre ->getBeginLoc(), DiagID).AddString(FD -> getNameAsString());
      }
    
    } 
     
  }
             
 
};
   

int main(int argc, const char **argv) {

  auto ExpectedParser = CommonOptionsParser::create(argc, argv, simple_integer_types);

  if (!ExpectedParser) {
    // Fail gracefully for unsupported options.
    errs() << ExpectedParser.takeError();
    return 1;
  }

  CommonOptionsParser &OptionsParser = ExpectedParser.get();
  ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());

  Simple_Pass_Handler Handler;
  MatchFinder Finder;
  Finder.addMatcher(callExprDeclaration, &Handler);
 
  Tool.run(newFrontendActionFactory(&Finder).get());


}