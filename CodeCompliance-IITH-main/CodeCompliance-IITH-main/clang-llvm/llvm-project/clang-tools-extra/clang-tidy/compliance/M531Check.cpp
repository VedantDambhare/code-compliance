//===--- M531Check.cpp - clang-tidy ---------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "M531Check.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::compliance {

void M531Check::registerMatchers(MatchFinder *Finder) {
  // FIXME: Add matchers.
  Finder->addMatcher(castExpr(hasCastKind(CK_IntegralToBoolean),
             hasParent(expr(anyOf(binaryOperator(hasAnyOperatorName("||", "&&","<","<=",">",">=","==","!=")), unaryOperator(hasOperatorName("!")))))).bind("intToBoolean"), this);
}

void M531Check::check(const MatchFinder::MatchResult &Result) {
  // FIXME: Add callback implementation.
    const CastExpr *castExpr = Result.Nodes.getNodeAs<CastExpr>("intToBoolean");
    // Get the location of the cast in the source code
    SourceLocation loc = castExpr->getBeginLoc();
    // Get the full location of the cast in the source code
    FullSourceLoc fullLoc(loc, (Result.Context->getSourceManager()));
    // Get the diagnostics engine to report errors
    DiagnosticsEngine &DE = Result.Context->getDiagnostics();
    // Get a custom error ID for the cast from int to bool violation
    const unsigned ID = DE.getCustomDiagID(
        clang::DiagnosticsEngine::Error,
        "MISRA C++ Rule 5.3.1 Violation! Each operand of the ! operator, the logical && or the logical || operators shall have type bool.");

    // Report the cast from int to bool violation
    DE.Report(loc, ID);
}

} // namespace clang::tidy::compliance
