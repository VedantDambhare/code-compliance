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

        while(true){
            pp.Lex(tok);
            if (tok.is(clang::tok::eof))
                break;

            // auto loc = sm.getSpellingLoc(tok.getLocation());
            // auto line = sm.getSpellingLineNumber(loc);
            tokens.push_back(tok);

        }

        unsigned int i = 0;
        while ( i < tokens.size()) {

            clang::tok::TokenKind tmp = tokens[i].getKind();
            int holder = i;
            //outs() << "Token: " << tokens[i].getName() << "\t" << clang::tok::getTokenName(tokens[i].getKind()) << "\n"; 
            if (tmp == clang::tok::utf8_string_literal || tmp == clang::tok::utf16_string_literal || tmp == clang::tok::wide_string_literal) {
                
                std::set<clang::tok::TokenKind> set_token_kind;
                set_token_kind.insert(tmp);
                
                while (true) {
                    i = i + 1;
                    clang::tok::TokenKind next_token = tokens[i].getKind();
                    //outs() << "Token: " << tokens[i].getName() << "\t" << clang::tok::getTokenName(tokens[i].getKind()) << "\n"; 
                    if ( next_token == clang::tok::utf8_string_literal || next_token == clang::tok::utf16_string_literal || next_token == clang::tok::wide_string_literal) {
                        set_token_kind.insert(next_token);
                    }
                    else if (next_token == clang::tok::string_literal) {
                        continue;
                    }
                    else {
                        break;
                    }
                }
                if (set_token_kind.size() > 1) {
                    auto loc = sm.getSpellingLoc(tokens[holder].getLocation());
                    auto line = sm.getSpellingLineNumber(loc);
                    diags.Report(loc, diags.getCustomDiagID(clang::DiagnosticsEngine::Error, "AUTOSAR C++ Rule A2.13.2: Violation! String literals with different encoding prefixes shall not be concatenated.")) << line;                    
                
                }
            }
            i = i + 1;
            
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
