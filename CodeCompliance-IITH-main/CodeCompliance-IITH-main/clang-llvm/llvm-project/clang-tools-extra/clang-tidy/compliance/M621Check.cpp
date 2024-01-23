//===--- M621Check.cpp - clang-tidy ---------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "M621Check.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::compliance {

void M621Check::registerMatchers(MatchFinder *Finder) {
  // FIXME: Add matchers.
  Finder->addMatcher(ifStmt(hasCondition(hasDescendant(binaryOperator(hasOperatorName("="))))).bind("if-node"), this);
  Finder->addMatcher(binaryOperator(hasOperatorName("="),hasDescendant(declRefExpr())).bind("decl-ref"), this);
}

void M621Check::check(const MatchFinder::MatchResult &Result) {
  // FIXME: Add callback implementation.
   DiagnosticsEngine &Diagnostics = Result.Context->getDiagnostics();
    
    if (const clang::IfStmt *st = Result.Nodes.getNodeAs<clang::IfStmt>("if-node")) {
        unsigned DiagID = Diagnostics.getCustomDiagID(DiagnosticsEngine::Error, "AUTOSAR C++ (Rule M6-2-1): Violated Assignment operators shall not be used in sub-expressions. ");
        Diagnostics.Report(st ->getBeginLoc(), DiagID);
    }
    else if (const clang::BinaryOperator *st = Result.Nodes.getNodeAs<clang::BinaryOperator>("decl-ref")) {
        unsigned DiagID = Diagnostics.getCustomDiagID(DiagnosticsEngine::Error, "AUTOSAR C++ (Rule M6-2-1): Violated Assignment operators shall not be used in sub-expressions. ");
        Diagnostics.Report(st ->getBeginLoc(), DiagID);
    }
}

} // namespace clang::tidy::compliance
