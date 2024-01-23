//===--- A741Check.cpp - clang-tidy ---------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//include necessary header files 
#include "A741Check.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::compliance {

void A741Check::registerMatchers(MatchFinder *Finder) {
  // FIXME: Add matchers.
  //write matcher to match asm statements 
   Finder->addMatcher(asmStmt().bind("gccAsmStmt"), this);
}

void A741Check::check(const MatchFinder::MatchResult &Result) {
  // FIXME: Add callback implementation.
  DiagnosticsEngine &DE = Result.Context->getDiagnostics();
  //Use matcher to match asm statements 
  const GCCAsmStmt *asmStmt = Result.Nodes.getNodeAs<GCCAsmStmt>("gccAsmStmt");
  //if match is not found in main file then ignore 
  if (!Result.SourceManager->isInMainFile(asmStmt -> getBeginLoc())) {
        return;
  }
  //if match is found in main file then report error 
  unsigned ID = DE.getCustomDiagID(
      DiagnosticsEngine::Error,
      "AUTOSAR C++ Rule A7-4-1 : Violated");

  DE.Report(asmStmt->getBeginLoc(), ID);
}

} // namespace clang::tidy::compliance
