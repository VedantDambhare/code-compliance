//===--- A723Check.cpp - clang-tidy ---------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "A723Check.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::compliance {

void A723Check::registerMatchers(MatchFinder *Finder) {
  // FIXME: Add matchers.
  Finder->addMatcher(enumDecl().bind("enumDecl"), this);
}

void A723Check::check(const MatchFinder::MatchResult &Result) {
  // FIXME: Add callback implementation.
   if(const EnumDecl *enumDecl = Result.Nodes.getNodeAs<EnumDecl>("enumDecl")){
        SourceLocation declLoc = enumDecl->getBeginLoc();
        if (!Result.SourceManager->isInMainFile(declLoc)) {
        return;
        }
        if(enumDecl->isScoped()) return;
        //Use the below 2 lines if only global enum's need to be checked
        // const DeclContext *context = enumDecl->getDeclContext();
        // if(context->isTranslationUnit() or context->isNamespace()){
        DiagnosticsEngine &DE = Result.Context->getDiagnostics();
        unsigned ID = DE.getCustomDiagID(
            DiagnosticsEngine::Error, "Rule A 7-2-3. Enumerations shall be declared as scoped enum classes.");
        DE.Report(enumDecl->getBeginLoc(), ID);
        }
}

} // namespace clang::tidy::compliance
