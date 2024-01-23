

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
DeclarationMatcher FunctionDeclMatcher = functionDecl(hasAncestor(callExpr().bind("callE"))).bind("function");



class Graph {
    public:
        map<std::string, std::list<std::string> > adj;
        
        void addEdge(std::string v, std::string w) {
             adj[v].push_back(w);
        }

        void printEdges() {
              for (auto i : adj) {
                  cout << i.first << endl;
                  std::list<std::string> tmp = i.second;
                  
                  for (auto j : tmp) {
                    cout << j << ' ';
                  }
                  std::cout << "\n";
              }

        }
};



class Simple_Pass_Handler : public MatchFinder::MatchCallback {
public:
 
  virtual void run(const MatchFinder::MatchResult &Result) override {
    DiagnosticsEngine &Diagnostics = Result.Context->getDiagnostics();
    
    if (const clang::FunctionDecl *st = Result.Nodes.getNodeAs<clang::FunctionDecl>("function")){
        const clang::CallExpr *ce =  Result.Nodes.getNodeAs<clang::CallExpr>("callE");
        std::pair<std::string,std::string> tmp(st->getNameAsString(),);
        edges.push_back(st ->getNameAsString());   
    }
     
  }
  private:
  std::vector<std::pair<std::string,std::string>> edges;
  

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
  Finder.addMatcher(FunctionDeclMatcher, &Handler);

  Tool.run(newFrontendActionFactory(&Finder).get());


}