//===--- A012Check.cpp - clang-tidy ---------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

//include necessary header files 
#include "A012Check.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::compliance {

void A012Check::registerMatchers(MatchFinder *Finder) {
  // FIXME: Add matchers.
  //Write function call matcher which does not have a void return type and whose value  returned is not used.
  StatementMatcher callExprDeclaration = callExpr(callee(functionDecl(unless(isNoReturn()))), unless(hasAncestor(varDecl())),unless(hasAncestor(ifStmt())),unless(hasAncestor(forStmt())),unless(hasAncestor(whileStmt())),unless(hasAncestor(returnStmt())),unless(hasAncestor(cxxStaticCastExpr()))).bind("function_call");
  Finder ->addMatcher(callExprDeclaration, this);
}

void A012Check::check(const MatchFinder::MatchResult &Result) {
  // FIXME: Add callback implementation.
  //Use function call matcher which does not have a void return type
  if (const clang::CallExpr *expre = Result.Nodes.getNodeAs<clang::CallExpr>("function_call")){
      SourceLocation expreLoc = expre->getBeginLoc();
      if (!Result.SourceManager->isInMainFile(expreLoc)) {
        return;
      }
      //Get function declaration from function call
      const clang::FunctionDecl *FD = expre -> getDirectCallee();
     //Get reuturn type of the function
      clang::QualType Qual = FD -> getReturnType();

    //if return type is not void and value returned is not used print error
      if ( (Qual.getTypePtr() -> isVoidType()) == false ) {
        //initialize diagnostic engine
        DiagnosticsEngine &Diagnostics = Result.Context->getDiagnostics();
        unsigned DiagID = Diagnostics.getCustomDiagID(DiagnosticsEngine::Error, " AUTOSAR C++ (Rule A0-1-2): Violated ");
        //report error message
        Diagnostics.Report(expre ->getBeginLoc(), DiagID).AddString(FD -> getNameAsString());
      }
    
    } 
}

} // namespace clang::tidy::compliance
