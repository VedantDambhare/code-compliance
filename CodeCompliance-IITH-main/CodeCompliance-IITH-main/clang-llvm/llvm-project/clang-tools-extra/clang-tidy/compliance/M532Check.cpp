//===--- M532Check.cpp - clang-tidy ---------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "M532Check.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::compliance {

void M532Check::registerMatchers(MatchFinder *Finder) {
  // FIXME: Add matchers.
  Finder->addMatcher(varDecl(
  hasType(isUnsignedInteger()),
  hasInitializer(ignoringParenImpCasts(unaryOperator(hasOperatorName("-"))))
).bind("unsignedVarDecl"), this);
}

void M532Check::check(const MatchFinder::MatchResult &Result) {
  // FIXME: Add callback implementation.
    // Get the matched var decl node
    const VarDecl *varDecl = Result.Nodes.getNodeAs<VarDecl>("unsignedVarDecl");
    // Get the location of the var decl in the source code
    SourceLocation loc = varDecl->getBeginLoc();
    // Get the full location of the var decl in the source code
    FullSourceLoc fullLoc(loc, (Result.Context->getSourceManager()));
    // Get the diagnostics engine to report errors
    DiagnosticsEngine &DE = Result.Context->getDiagnostics();
    // Get a custom error ID for the unsigned variable with negation violation
    const unsigned ID = DE.getCustomDiagID(
        clang::DiagnosticsEngine::Error,
        "MISRA C++ Rule 5.3.3 Violation! The unary minus operator shall not be applied to an operand whose underlying type is unsigned.");

    // Report the unsigned variable with negation violation
    DE.Report(loc, ID);

}
} // namespace clang::tidy::compliance
