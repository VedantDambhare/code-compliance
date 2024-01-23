//===--- M732Check.cpp - clang-tidy ---------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//include necessary header files 
#include "M732Check.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::compliance {

void M732Check::registerMatchers(MatchFinder *Finder) {
  // FIXME: Add matchers.
  //Add matcher to match function declarations 
  Finder->addMatcher(functionDecl().bind("funcDecl"), this);
}

void M732Check::check(const MatchFinder::MatchResult &Result) {
  // FIXME: Add callback implementation.
  //Use function declaration matcher 
  const FunctionDecl *decl = Result.Nodes.getNodeAs<FunctionDecl>("funcDecl");
  SourceLocation declLoc = decl->getBeginLoc();
  //if match is not found in main file then ignore 
  if (!Result.SourceManager->isInMainFile(declLoc)) {
    return;
  }
  //if function by name main is found increment counter 
  if (decl->getNameAsString() == "main") {
    i++;
  } else {
    return;
  }
  //if more than one main functions are encounterede then report error 
  if (i > 1) {
    DiagnosticsEngine &Diags = Result.Context->getDiagnostics();
    unsigned ID = Diags.getCustomDiagID(
        DiagnosticsEngine::Error,
        "AUTOSAR C++ Rule M 7-3-2 : Violated ");
    Diags.Report(decl->getLocation(), ID);
  }
}

} // namespace clang::tidy::compliance
