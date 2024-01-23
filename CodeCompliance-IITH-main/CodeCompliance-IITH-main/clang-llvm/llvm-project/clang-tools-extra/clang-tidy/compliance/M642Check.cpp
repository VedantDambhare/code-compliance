//===--- M642Check.cpp - clang-tidy ---------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//include necessary header files 
#include "M642Check.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::compliance {

void M642Check::registerMatchers(MatchFinder *Finder) {
  // FIXME: Add matchers.
  //write matcher to match if else construct 
  Finder->addMatcher(ifStmt(hasAncestor(ifStmt())).bind("node"), this);
}

void M642Check::check(const MatchFinder::MatchResult &Result) {
  // FIXME: Add callback implementation.
  //initialize diagnostic engine 
  DiagnosticsEngine &DE = Result.Context->getDiagnostics();
  //use matcher to match if else construct 
  const clang::IfStmt *st = Result.Nodes.getNodeAs<clang::IfStmt>("node");
  // if match is found in any file other than main then ignore 
  if (!Result.SourceManager->isInMainFile(st -> getBeginLoc())) {
        return;
  }
  //if the if else construct has no else the report error message 
  if (st->hasElseStorage() == false) {

    unsigned DiagID = DE.getCustomDiagID(
        DiagnosticsEngine::Error,
        "AUTOSAR C++ (Rule M6-4-2): Violated ");
    DE.Report(st->getBeginLoc(), DiagID);
  }


}

} // namespace clang::tidy::compliance
