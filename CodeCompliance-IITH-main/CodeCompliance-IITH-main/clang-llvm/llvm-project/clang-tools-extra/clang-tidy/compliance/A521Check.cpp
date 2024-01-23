//===--- A521Check.cpp - clang-tidy ---------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "A521Check.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::compliance {

void A521Check::registerMatchers(MatchFinder *Finder) {
  // FIXME: Add matchers.
  Finder->addMatcher(cxxDynamicCastExpr().bind("dynamicCast"), this);
}

void A521Check::check(const MatchFinder::MatchResult &Result) {
  // FIXME: Add callback implementation.
     if (const CXXDynamicCastExpr *DynCast =
            Result.Nodes.getNodeAs<CXXDynamicCastExpr>("dynamicCast")) {
            DiagnosticsEngine &DE = Result.Context->getDiagnostics();      
            const unsigned ID = DE.getCustomDiagID(         
            DiagnosticsEngine::Error, "AUTOSAR C++ Rule A-5-2-1 violated."
            );
            DE.Report(DynCast->getBeginLoc(), ID);
}
}

} // namespace clang::tidy::compliance
