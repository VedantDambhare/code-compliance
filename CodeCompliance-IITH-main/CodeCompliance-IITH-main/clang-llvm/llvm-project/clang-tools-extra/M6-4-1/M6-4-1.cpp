

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
// StatementMatcher StatementMatch = ifStmt(hasAncestor(ifStmt())).bind("node");
StatementMatcher StatementMatch = nullStmt(hasParent(ifStmt())).bind("node");
StatementMatcher StatementMatch1 = ifStmt().bind("if-node");
// StatementMatcher sm = anyOf(declaratorDecl().bind("nd"),callExpr().bind("nd"),declStmt().bind("nd"));

StatementMatcher ExpressionMatcher = expr().bind("exprNode");
StatementMatcher StmtMatcher = stmt().bind("stmtNode");
DeclarationMatcher DeclMatcher = decl().bind("declNode");
TypeMatcher TypeMatch = type().bind("typeNode");


class Simple_Pass_Handler : public MatchFinder::MatchCallback {
public:
 
  virtual void run(const MatchFinder::MatchResult &Result) override {
    DiagnosticsEngine &Diagnostics = Result.Context->getDiagnostics();
    
    if (const clang::NullStmt *st = Result.Nodes.getNodeAs<clang::NullStmt>("node")){
        
        unsigned DiagID = Diagnostics.getCustomDiagID(DiagnosticsEngine::Error, "AUTOSAR C++ (Rule M6-4-1): Violated An if ( condition ) construct shall be followed by a compound statement.The else keyword shall be followed by either a compound statement, or another if statement. ");
        Diagnostics.Report(st ->getBeginLoc(), DiagID);
      
    }
    else if (const clang::IfStmt *st = Result.Nodes.getNodeAs<clang::IfStmt>("if-node")) {
        
        clang::SourceLocation sl_2 = st -> getEndLoc();
        clang::FullSourceLoc get_sl_2 = Result.Context->getFullLoc(sl_2);
        ElseDetails.push_back(get_sl_2.getLineNumber());

    }
  
  }

  const std::vector<unsigned int>& getModifiedVars() const {
    return ElseDetails;
  }


private:
  std::vector<unsigned int> ElseDetails;
     
};


class CaseChecker : public MatchFinder::MatchCallback {
public:
  CaseChecker(const std::vector<unsigned int>& variablesSet) : variablesSet(variablesSet) {}
 
  virtual void run(const MatchFinder::MatchResult &Result) override {

        DiagnosticsEngine &Diagnostics = Result.Context->getDiagnostics();
        clang::SourceLocation sl_2;
        clang::FullSourceLoc get_sl;
         
        if (const clang::Stmt *st = Result.Nodes.getNodeAs<clang::Stmt>("stmtNode")) {
          sl_2 = st -> getBeginLoc();
          get_sl = Result.Context->getFullLoc(sl_2);
        }
        else if (const clang::Expr *st = Result.Nodes.getNodeAs<clang::Expr>("exprNode")) {
          sl_2 = st -> getBeginLoc();
          get_sl = Result.Context->getFullLoc(sl_2);
        }
        else if (const clang::Decl *st = Result.Nodes.getNodeAs<clang::Decl>("declNode")) {
          sl_2 = st -> getBeginLoc();
          get_sl = Result.Context->getFullLoc(sl_2);
        }
        
        int ElseIssues = 0;
        for (const unsigned int stmt : variablesSet) {
          if (((stmt - get_sl.getLineNumber()) == 1) ) {
            ElseIssues = 1;
          } 
        }

        if (ElseIssues == 1) {
          unsigned DiagID = Diagnostics.getCustomDiagID(DiagnosticsEngine::Error, "AUTOSAR C++ (Rule M6-4-1): Violated An if ( condition ) construct shall be followed by a compound statement.The else keyword shall be followed by either a compound statement, or another if statement. ");
          Diagnostics.Report(sl_2,DiagID);
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
  Tool.run(newFrontendActionFactory(&Finder).get());
  const std::vector<unsigned int> modifiedVarsSet = Handler.getModifiedVars();

  MatchFinder NFinder;
  CaseChecker Checker2(modifiedVarsSet); 
  NFinder.addMatcher(ExpressionMatcher,&Checker2);
  NFinder.addMatcher(StmtMatcher,&Checker2);
  NFinder.addMatcher(DeclMatcher,&Checker2);
  
  
  Tool.run(newFrontendActionFactory(&NFinder).get());

}