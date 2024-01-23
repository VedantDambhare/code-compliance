//===--- A716Check.cpp - clang-tidy ---------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "A716Check.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::compliance {
//write matcher to match typedef specifiers
void A716Check::registerMatchers(MatchFinder *Finder) {
  // FIXME: Add matchers.
  //write a matcher to match typedef declaration 
  Finder->addMatcher(typedefDecl().bind("typedef"), this);
}

void A716Check::check(const MatchFinder::MatchResult &Result) {
  // FIXME: Add callback implementation.

  //Initialize diagnostic engine 
  DiagnosticsEngine &DE = Result.Context->getDiagnostics();
  const TypedefDecl *typedefDecl =
  Result.Nodes.getNodeAs<TypedefDecl>("typedef");
  SourceLocation loc = typedefDecl->getBeginLoc();
  //if match is not in main file then ignore 
  if (!Result.SourceManager->isInMainFile(loc)) {
    return;
  }
//if match is found the report error 
  unsigned ID = DE.getCustomDiagID(
      DiagnosticsEngine::Error,
      "AUTOSAR C++ Rule A 7-1-6 : Violated");
  DE.Report(loc, ID);

}

} // namespace clang::tidy::compliance
