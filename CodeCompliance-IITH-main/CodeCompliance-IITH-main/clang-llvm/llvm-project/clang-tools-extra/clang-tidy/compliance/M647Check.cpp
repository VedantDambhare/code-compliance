//===--- M647Check.cpp - clang-tidy ---------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "M647Check.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::compliance {

void M647Check::registerMatchers(MatchFinder *Finder) {
  // FIXME: Add matchers.
  Finder->addMatcher( implicitCastExpr(
        unless(isExpansionInSystemHeader()),  //System header ignored
        hasCastKind(CK_IntegralCast),
        hasParent(switchStmt().bind("switchStmt"))
    ).bind("implicitCastExpr"), this);
}

void M647Check::check(const MatchFinder::MatchResult &Result) {
  // FIXME: Add callback implementation.
    // Get the matched switch statement node
    const SwitchStmt *switchStmt = Result.Nodes.getNodeAs<SwitchStmt>("switchStmt");
    // Get the location of the switch statement in the source code
    SourceLocation loc = switchStmt->getBeginLoc();
    // Get the full location of the switch statement in the source code
    FullSourceLoc fullLoc(loc, (Result.Context->getSourceManager()));
    // Get the diagnostics engine to report errors
    DiagnosticsEngine &DE = Result.Context->getDiagnostics();
    // Get a custom error ID for the Integral cast within the switch statement violation
    const unsigned ID = DE.getCustomDiagID(
        clang::DiagnosticsEngine::Error,
        "MISRA C++ Rule 6.4.7 Violation!  The condition of a switch statement shall not have bool type.");

    // Report the Integral cast within the switch statement violation
    DE.Report(loc, ID);
}

} // namespace clang::tidy::compliance
