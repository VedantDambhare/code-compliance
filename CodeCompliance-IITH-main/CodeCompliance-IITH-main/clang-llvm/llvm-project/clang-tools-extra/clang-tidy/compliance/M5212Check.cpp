//===--- M5212Check.cpp - clang-tidy --------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "M5212Check.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::compliance {

void M5212Check::registerMatchers(MatchFinder *Finder) {
  // FIXME: Add matchers.
  Finder->addMatcher(callExpr().bind("callExpr"), this);
}

void M5212Check::check(const MatchFinder::MatchResult &Result) {
  // FIXME: Add callback implementation.
  if(const CallExpr *callExpr = Result.Nodes.getNodeAs<CallExpr>("callExpr")){
      SourceLocation loc = callExpr->getBeginLoc();
      if(!Result.SourceManager->isInMainFile(loc)) {
        return;
      }
      for (unsigned i = 0; i < callExpr->getNumArgs(); ++i) {
        const Expr *argExpr = callExpr->getArg(i);
        QualType argType = argExpr->getType();
        if (const DeclRefExpr *declRef = dyn_cast<DeclRefExpr>(argExpr->IgnoreImpCasts())) {
          if (declRef->getType()->isArrayType() and argType->isPointerType()) {
            DiagnosticsEngine &DE = Result.Context->getDiagnostics();
            unsigned ID = DE.getCustomDiagID(
                DiagnosticsEngine::Error, "Rule M 5-2-12. An identifier with array type passed as a function argument shall not decay to a pointer."
                );
            DE.Report(loc, ID);
          } 
        } 
      }
  }
}

} // namespace clang::tidy::compliance
