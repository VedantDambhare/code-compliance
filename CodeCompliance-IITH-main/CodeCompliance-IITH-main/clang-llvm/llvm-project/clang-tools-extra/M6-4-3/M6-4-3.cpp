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


static llvm::cl::OptionCategory Simple_Pass("Simple_Pass options");

// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

// A help message for this specific tool can be added afterwards.
static cl::extrahelp MoreHelp("\nMore help text...\n");


StatementMatcher StatementMatch = switchStmt( unless(hasDescendant(defaultStmt()))).bind("snode");
StatementMatcher StatementMatch_c = caseStmt( unless(hasDescendant(caseStmt())) ).bind("cnode");




class Simple_Pass_Handler : public MatchFinder::MatchCallback {
public:

    virtual void run(const MatchFinder::MatchResult &Result) override {
        DiagnosticsEngine &Diagnostics = Result.Context->getDiagnostics();
        if (const clang::CaseStmt *st = Result.Nodes.getNodeAs<clang::CaseStmt>("cnode")) {
            clang::DynTypedNodeList NodeList = Result.Context->getParents(*st);            
            int flag = 0;
            if (!NodeList.empty()) {
                clang::DynTypedNode ParentNode = NodeList[0];
                if (const CompoundStmt *Parent = ParentNode.get<CompoundStmt>()) {
                    for (const Stmt *childStmt : Parent->children()) {
                        if (childStmt == st) {
                            flag = 1;
                        }
                        else if (isa<BreakStmt>(childStmt) && flag == 1) {
                            flag = 0;
                            break;
                        }
                        else if (isa<CaseStmt>(childStmt) && flag == 1) {
                            unsigned DiagID = Diagnostics.getCustomDiagID(DiagnosticsEngine::Error, "AUTOSAR C++ (Rule M6-4-3): Violated Switch statement should be well formed switch statement ");
                            Diagnostics.Report(st ->getBeginLoc(), DiagID);  
                        } 
                        // If childStmt = st (our case stmt) flag = 1
                        // If childStmt = Break and flag = 1: flag = 0
                        // If childStmt = CaseStmt and flag = 1 raise error

                    }
                }
            }
        }
        else if (const clang::SwitchStmt *st = Result.Nodes.getNodeAs<clang::SwitchStmt>("snode")){

            unsigned DiagID = Diagnostics.getCustomDiagID(DiagnosticsEngine::Error, "AUTOSAR C++ (Rule M6-4-3): Violated Switch statement should be well formed switch statement ");
            Diagnostics.Report(st ->getBeginLoc(), DiagID);
      
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
    Finder.addMatcher(StatementMatch, &Handler);
    Finder.addMatcher(StatementMatch_c, &Handler);

    Tool.run(newFrontendActionFactory(&Finder).get());

}