

#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include <iostream>
#include "clang/Tooling/Inclusions/HeaderIncludes.h"
#include "clang/Tooling/Core/Replacement.h"
#include <map>

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
static llvm::cl::OptionCategory Simple_Pass("Simple_Pass options");

// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

// A help message for this specific tool can be added afterwards.
static cl::extrahelp MoreHelp("\nMore help text...\n");

// Define a matcher for switch case statements.
// StatementMatcher LabelStmtMatcher = labelStmt().bind("LabelStmt");
// StatementMatcher GotoStmtMatcher =() gotoStmt().bind("GotoStmt");
// DeclarationMatcher FunctionDeclMatcher = functionDecl().bind("function");
// Matcher to find all header includes.
StatementMatcher StatementMatch = switchStmt( unless(hasDescendant(defaultStmt()))).bind("snode");
StatementMatcher StatementMatch_c = caseStmt( unless(hasDescendant(caseStmt())) ).bind("cnode");
StatementMatcher StatementMatch_b = breakStmt().bind("bnode");
StatementMatcher StatementMatch_d = defaultStmt().bind("dnode");
    
class Simple_Pass_Handler : public MatchFinder::MatchCallback {
public:
 
  virtual void run(const MatchFinder::MatchResult &Result) override {
    DiagnosticsEngine &Diagnostics = Result.Context->getDiagnostics();
    
    if (const clang::SwitchStmt *st = Result.Nodes.getNodeAs<clang::SwitchStmt>("snode")){

        unsigned DiagID = Diagnostics.getCustomDiagID(DiagnosticsEngine::Error, "AUTOSAR C++ (Rule M6-4-3): Violated Switch statement should be well formed switch statement ");
        Diagnostics.Report(st ->getBeginLoc(), DiagID);
      
    }
    else if (const clang::BreakStmt *st = Result.Nodes.getNodeAs<clang::BreakStmt>("bnode")) {
    
      clang::SourceLocation sl = st -> getBeginLoc();
      clang::FullSourceLoc get_sl = Result.Context->getFullLoc(sl); 
      
      breakStmtDetails.push_back(get_sl.getLineNumber());
    }
     
  }

  const std::vector<unsigned int>& getModifiedVars() const {
    return breakStmtDetails;
  }

private:
  std::vector<unsigned int> breakStmtDetails;
};


class CaseChecker : public MatchFinder::MatchCallback {
public:
  CaseChecker(const std::vector<unsigned int>& variablesSet) : variablesSet(variablesSet) {}
 
  virtual void run(const MatchFinder::MatchResult &Result) override {
    if (const clang::CaseStmt *st = Result.Nodes.getNodeAs<clang::CaseStmt>("cnode")) {

      DiagnosticsEngine &Diagnostics = Result.Context->getDiagnostics();
      clang::SourceLocation sl = st -> getCaseLoc();
      clang::FullSourceLoc get_sl = Result.Context->getFullLoc(sl);  
      
      int breakPresent = 0;

      for (const unsigned int stmt : variablesSet) {
      

        if (((stmt - get_sl.getLineNumber()) == 1) || ((stmt - get_sl.getLineNumber()) == 2) ) {
            breakPresent = 1;
        } 

      }
      if (breakPresent == 0) {
        unsigned DiagID = Diagnostics.getCustomDiagID(DiagnosticsEngine::Error, "AUTOSAR C++ (Rule M6-4-3): Violated Switch statement should be well formed switch statement ");
        Diagnostics.Report(st ->getBeginLoc(), DiagID);
      }
    }

    else if (const clang::DefaultStmt *st = Result.Nodes.getNodeAs<clang::DefaultStmt>("dnode")) {

      DiagnosticsEngine &Diagnostics = Result.Context->getDiagnostics();
      clang::SourceLocation sl = st -> getDefaultLoc();
      clang::FullSourceLoc get_sl = Result.Context->getFullLoc(sl);  
      
      int breakPresent = 0;

      for (const unsigned int stmt : variablesSet) {
      

        if (((stmt - get_sl.getLineNumber()) == 1) || ((stmt - get_sl.getLineNumber()) == 2) ) {
            breakPresent = 1;
        } 

      }
      if (breakPresent == 0) {
        unsigned DiagID = Diagnostics.getCustomDiagID(DiagnosticsEngine::Error, "AUTOSAR C++ (Rule M6-4-3): Violated Switch statement should be well formed switch statement ");
        Diagnostics.Report(st ->getBeginLoc(), DiagID);
      }
    }
    
  }
 
private:
  const std::vector<unsigned int>& variablesSet;
};

  





int main(int argc, const char **argv) {

  auto ExpectedParser = CommonOptionsParser::create(argc, argv, Simple_Pass);

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
  Finder.addMatcher(StatementMatch, &Handler);
  Finder.addMatcher(StatementMatch_b,&Handler);

  Tool.run(newFrontendActionFactory(&Finder).get());
  const std::vector<unsigned int> modifiedVarsSet = Handler.getModifiedVars();


  MatchFinder breakFinder;
  CaseChecker Checker2(modifiedVarsSet); 
  breakFinder.addMatcher(StatementMatch_c,&Checker2);
  breakFinder.addMatcher(StatementMatch_d,&Checker2);
  Tool.run(newFrontendActionFactory(&breakFinder).get());

}