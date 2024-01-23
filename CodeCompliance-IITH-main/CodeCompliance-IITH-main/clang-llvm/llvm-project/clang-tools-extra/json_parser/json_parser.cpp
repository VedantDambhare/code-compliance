#include "clang/Frontend/FrontendActions.h"
#include "clang/Lex/Lexer.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/raw_ostream.h"
#include "clang/Frontend/CompilerInstance.h"
#include "llvm/Support/CommandLine.h"

using namespace clang;
using namespace clang::tooling;
using namespace llvm;
using namespace std;


class StringLiteralFinder : public clang::PreprocessorFrontendAction {
    public:
    virtual void ExecuteAction() {
        auto& pp = getCompilerInstance().getPreprocessor();
        auto& diags = getCompilerInstance().getDiagnostics();
        auto &sm = pp.getSourceManager();

        pp.EnterMainSourceFile();
        clang::Token tok;


        std::vector<clang::Token> tokens;
        int i = 0;
        int j = 0;
        // Push all the tokens to the vector
        while(true){
            pp.Lex(tok);
            if (tok.is(clang::tok::eof))
                break;
            clang::tok::TokenKind token_kind = tok.getKind();
            if (token_kind == clang::tok::l_brace) {
                i += 1;
            }
            else if (token_kind == clang::tok::r_brace) {
                j += 1;
            }
        }

        if (i < 1 || j < 1) {
            diags.Report(diags.getCustomDiagID(clang::DiagnosticsEngine::Error, "Invalid JSON file"));
        }
        

    }

};
// Define an option category for the tool
static llvm::cl::OptionCategory MyToolCategory("my-tool options");
int main(int argc, const char** argv) {
    auto ExpectedParser = CommonOptionsParser::create(argc, argv,MyToolCategory);
    if (!ExpectedParser) {
        llvm::errs() << ExpectedParser.takeError();
        return 1;
    }
    CommonOptionsParser& OptionsParser = ExpectedParser.get();
    ClangTool Tool(OptionsParser.getCompilations(),OptionsParser.getSourcePathList());
    return Tool.run(newFrontendActionFactory<StringLiteralFinder>().get());

}