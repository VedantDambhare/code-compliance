

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
DeclarationMatcher MethodDeclMatcher = cxxMethodDecl(isVirtual()).bind("method");

    
class Simple_Pass_Handler : public MatchFinder::MatchCallback {
public:
 
  virtual void run(const MatchFinder::MatchResult &Result) override {
     DiagnosticsEngine &Diagnostics = Result.Context->getDiagnostics();
     
     if (const CXXMethodDecl *RD = Result.Nodes.getNodeAs<CXXMethodDecl>("method")) { 

        for(CXXMethodDecl::method_iterator MI = RD->begin_overridden_methods(),ME = RD->end_overridden_methods(); MI != ME; ++MI) {
            
            const CXXMethodDecl *method_o = *MI;
            for(FunctionDecl::param_const_iterator PI = RD->param_begin(),PE = RD->param_end(); PI != PE; ++PI) { 
                int found = 0;
                for(FunctionDecl::param_const_iterator PI_2 = method_o ->param_begin(), PE_2 = method_o -> param_end(); PI_2 != PE_2; ++PI_2) {
                    const ParmVarDecl * param_1 = *PI;
                    const ParmVarDecl * param_2 = *PI_2;
                    if (param_1 -> getNameAsString() == param_2 -> getNameAsString()) {
                        found = 1;
                    }
                }
                if (found == 0){
                    unsigned DiagID = Diagnostics.getCustomDiagID(DiagnosticsEngine::Error, " AUTOSAR C++ (Rule A0-1-5): Violated - There shall be no unused named parameters in the set of parameters for a virtual function and all the functions that override it.");
                    Diagnostics.Report(RD ->getBeginLoc(), DiagID);          
                }

            }

        }
        
        
        // CXXRecordDecl ParentClass = RD -> getParent();
        // CXXMethodDecl baseMethod = RD -> getCorrespondingMethodDeclaredInClass(ParentClass);

        
        // for(FunctionDecl::param_const_iterator PI = RD->param_begin(),PE = RD->param_end(); PI != PE; ++PI) {  
        //         const ParmVarDecl * param = *PI;
        //         if (param -> isThisDeclarationReferenced() == false) {
                  
                    

        //         }
        // }

        // unsigned DiagID = Diagnostics.getCustomDiagID(DiagnosticsEngine::Error, " AUTOSAR C++ (Rule A0-1-5): Violated - There shall be no unused named parameters in the set of parameters for a virtual function and all the functions that override it.");
        // Diagnostics.Report(RD ->getBeginLoc(), DiagID);
        
        
     }
     
  }
     
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
  Finder.addMatcher(MethodDeclMatcher, &Handler);

  Tool.run(newFrontendActionFactory(&Finder).get());


}