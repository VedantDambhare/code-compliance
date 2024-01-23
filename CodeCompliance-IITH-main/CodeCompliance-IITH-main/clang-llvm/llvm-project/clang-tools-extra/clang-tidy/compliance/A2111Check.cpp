//===--- A2111Check.cpp - clang-tidy --------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

//include necessary header files
#include "A2111Check.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::compliance {

void A2111Check::registerMatchers(MatchFinder *Finder) {
  // FIXME: Add matchers.
  //write matchers to match the variables and members of class which are declared as volatile and report error if match found
  Finder->addMatcher(
      varDecl(hasType(isVolatileQualified())).bind("volatileVar"), this);
  
  Finder->addMatcher(fieldDecl(hasType(isVolatileQualified())).bind("field_vol"),
                     this);
}

void A2111Check::check(const MatchFinder::MatchResult &Result) {
  // FIXME: Add callback implementation.
  //Initialize diagnostic engine with error message 
  DiagnosticsEngine &DE = Result.Context->getDiagnostics();

  unsigned ID = DE.getCustomDiagID(DiagnosticsEngine::Error,
                                   "AUTOSAR CPP Rule (A2.11.1) : Violated");
  //use matcher to match the variables  which are declared as volatile
  if (const VarDecl *Var = Result.Nodes.getNodeAs<VarDecl>("volatileVar")) {
    // outs() << "Found volatile variable: " << Var->getNameAsString() <<"\n";

    //incase match is not in main file then ignore 
    if (!Result.SourceManager->isInMainFile(Var -> getBeginLoc())) {
        return;
    }
    DE.Report(Var->getBeginLoc(), ID);
    
//use matcher to match the members of class  which are declared as volatile and report error if match found
  } else if (const FieldDecl *FLD =
                 Result.Nodes.getNodeAs<FieldDecl>("field_vol")) {
    //incase match is not in main file then ignore 
      if (!Result.SourceManager->isInMainFile(FLD -> getBeginLoc())) {
        return;
      }
      DE.Report(FLD->getBeginLoc(), ID);
    
  }
}

} // namespace clang::tidy::compliance
