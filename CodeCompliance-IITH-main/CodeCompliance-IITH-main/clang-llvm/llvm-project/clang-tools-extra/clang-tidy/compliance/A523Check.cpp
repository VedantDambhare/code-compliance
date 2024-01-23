//===--- A523Check.cpp - clang-tidy ---------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//include necessary header files
#include "A523Check.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::compliance {

void A523Check::registerMatchers(MatchFinder *Finder) {
  // FIXME: Add matchers.
  //write matchers to match casting expressions
  Finder -> addMatcher(cxxConstCastExpr().bind("castExpr"), this);
  Finder -> addMatcher(cStyleCastExpr().bind("castExpr"), this);
}

void A523Check::check(const MatchFinder::MatchResult &Result) {
  // FIXME: Add callback implementation.
  //use matcher to match casting expressions for const cast
  if(const CXXConstCastExpr *castExpr = Result.Nodes.getNodeAs<CXXConstCastExpr>("castExpr")){
      SourceLocation loc = castExpr->getBeginLoc();
    //incase the match is not in main file then ignore 
      if (!Result.SourceManager->isInMainFile(loc)) {
        return;
      }
      bool flag = false, addressOf = false;
    //check all unary operators whether the cast is removing const or volatile qualification and set the flags accordingly 
      if (const UnaryOperator *unaryOp = dyn_cast<UnaryOperator>(castExpr->getSubExprAsWritten())) {
        if(unaryOp->getOpcode() == UO_AddrOf){ 
          std::string type = unaryOp->getType().getAsString();
          if (type.find("const") != std::string::npos) {
            addressOf = true;
          }else if (type.find("volatile") != std::string::npos) {
            addressOf = true;
          }else{
            addressOf = false;
          }
        }
      }
    //check all variable declarations whether the cast is removing const or volatile qualification and set the flags accordingly 
      if(const DeclRefExpr *varExpr = dyn_cast<DeclRefExpr>(castExpr->getSubExprAsWritten())) {
        if(varExpr->getType()->isPointerType() or varExpr->getType()->isReferenceType()){
          std::string type = varExpr->getType().getAsString();
          if (type.find("const") != std::string::npos) {
            flag = true;
          }else if (type.find("volatile") != std::string::npos) {
            flag = true;
          }else{
            flag = false;
          }
        }
      }
    //incase any of the flag is true report error using diagnostic engine 
      if(flag or addressOf){
        DiagnosticsEngine &DE = Result.Context->getDiagnostics();      
        const unsigned ID = DE.getCustomDiagID(         
          DiagnosticsEngine::Error, "AUTOSAR C++ Rule M 5-2-3 : Violated"
          );
        DE.Report(castExpr->getBeginLoc(), ID);
      }
    }
   //use matcher to match casting expressions for C style  cast
    if(const CStyleCastExpr *castExpr = Result.Nodes.getNodeAs<CStyleCastExpr>("castExpr")){
      SourceLocation loc = castExpr->getBeginLoc();
     //incase the match is not in main file then ignore 
      if (!Result.SourceManager->isInMainFile(loc)) {
        return;
      }
      bool flag = false, addressOf = false;
       //check all unary operators whether the C style cast is removing const or volatile qualification and set the flags accordingly 
      if (const UnaryOperator *unaryOp = dyn_cast<UnaryOperator>(castExpr->getSubExprAsWritten())) {
        if(unaryOp->getOpcode() == UO_AddrOf){ 
          std::string type = unaryOp->getType().getAsString();
          if (type.find("const") != std::string::npos) {
            addressOf = true;
          }else if (type.find("volatile") != std::string::npos) {
            addressOf = true;
          }else{
            addressOf = false;
          }
        }
      }
      //check all variable declarations whether the C style cast is removing const or volatile qualification and set the flags accordingly 
      if(const DeclRefExpr *varExpr = dyn_cast<DeclRefExpr>(castExpr->getSubExprAsWritten())) {
        if(varExpr->getType()->isPointerType() or varExpr->getType()->isReferenceType()){
          std::string type = varExpr->getType().getAsString();
          if (type.find("const") != std::string::npos) {
            flag = true;
          }else if (type.find("volatile") != std::string::npos) {
            flag = true;
          }else{
            flag = false;
          }
        }
      }
    //incase any of the flag is true report error using diagnostic engine 
      if(flag or addressOf){
        DiagnosticsEngine &DE = Result.Context->getDiagnostics();      
        const unsigned ID = DE.getCustomDiagID(         
          DiagnosticsEngine::Error, "Rule M 5-2-5 : Violated"
          );
        DE.Report(castExpr->getBeginLoc(), ID);
      }
    }
}

} // namespace clang::tidy::compliance
