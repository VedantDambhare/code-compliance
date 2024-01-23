//===--- M662Check.cpp - clang-tidy ---------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "M662Check.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::compliance {

void M662Check::registerMatchers(MatchFinder *Finder) {
  // FIXME: Add matchers.
  Finder -> addMatcher(gotoStmt().bind("goto"), this);
}

void M662Check::check(const MatchFinder::MatchResult &Result) {
  // FIXME: Add callback implementation.
  const GotoStmt *gotoStmt = Result.Nodes.getNodeAs<GotoStmt>("goto");
  const LabelDecl *labelStmt = gotoStmt->getLabel();
    SourceLocation labelLoc = labelStmt->getBeginLoc();
    SourceLocation gotoLoc = gotoStmt->getGotoLoc();

    if (Result.SourceManager->isBeforeInTranslationUnit(labelLoc, gotoLoc)) {
        DiagnosticsEngine &DE = Result.Context->getDiagnostics();      
        const unsigned ID = DE.getCustomDiagID(         
            DiagnosticsEngine::Error, "AUTOSAR C++ Rule M 6-6-2 : Violated"
            );
        DE.Report(gotoLoc, ID) << labelStmt->getName();
    }
}

} // namespace clang::tidy::compliance
