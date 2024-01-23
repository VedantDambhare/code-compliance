//===--- A724Check.cpp - clang-tidy ---------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//include necessary header files 
#include "A724Check.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::compliance {

void A724Check::registerMatchers(MatchFinder *Finder) {
  // FIXME: Add matchers.
  //Write matcher to match enum declaration 
  Finder -> addMatcher(enumDecl().bind("enumDecl"), this);
}

void A724Check::check(const MatchFinder::MatchResult &Result) {
  // FIXME: Add callback implementation.
  //use matcher to match enum declaration 
  if(const EnumDecl *enumDecl = Result.Nodes.getNodeAs<EnumDecl>("enumDecl")){
    SourceLocation declLoc = enumDecl->getBeginLoc();
    //if match is not found in main file then ignore 
    if (!Result.SourceManager->isInMainFile(declLoc)) {
      return;
    }
    //initialize counters to count number of enumerators initialized 
    int i = 0;
    int count = 0;
    bool one = false;
    bool none = true;
    //iterate through all enumerators 
    for(const EnumConstantDecl *enumConstant : enumDecl->enumerators()) {
        if(enumConstant->getInitExpr()){
            none = false;
            if(i==0){ 
              one = true;
            }else{
                count++;
            }
        }
        i++;
      }
    if(none) return;
    if(one and count==0) return;
    if(one and count==i-1) return;
    //incase other than none first or all enumerators there are some partial initializations then report error 
    DiagnosticsEngine &DE = Result.Context->getDiagnostics();
    unsigned ID = DE.getCustomDiagID(
        DiagnosticsEngine::Error, "AUTOSAR C++ Rule A 7-2-4 : Violated");
    DE.Report(enumDecl->getBeginLoc(), ID);
  }


}

} // namespace clang::tidy::compliance
