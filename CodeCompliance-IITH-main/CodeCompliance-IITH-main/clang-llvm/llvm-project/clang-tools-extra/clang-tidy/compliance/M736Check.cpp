//===--- M736Check.cpp - clang-tidy ---------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//include necessary header files 
#include "M736Check.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::compliance {

void M736Check::registerMatchers(MatchFinder *Finder) {
  // FIXME: Add matchers.
  //write matcher to match using directive 
  Finder->addMatcher(usingDirectiveDecl().bind("usingDirective"), this);
}

void M736Check::check(const MatchFinder::MatchResult &Result) {
  // FIXME: Add callback implementation.
  
  DiagnosticsEngine &DE = Result.Context->getDiagnostics();
  //initialize diagnostic engine with error message 
  const unsigned ID = DE.getCustomDiagID(
      DiagnosticsEngine::Error,
      "AUTOSAR C++ Rule M 7-3-6 : Violated ");
//match using directive 
  const UsingDirectiveDecl *UDD = Result.Nodes.getNodeAs<UsingDirectiveDecl>("usingDirective");
  //if match is found outside main file then ignore 
  if (!Result.SourceManager->isInMainFile(UDD ->getBeginLoc())) {
        return;
  }
  //if using directive exist 
  if (UDD) {

    SourceLocation loc = UDD->getBeginLoc();
    const FileEntry *fileEntry = Result.SourceManager->getFileEntryForID(
        Result.SourceManager->getFileID(loc));
    StringRef filename = fileEntry->getName();
    //check if main file is header file 
    if (filename.endswith(".h") || filename.endswith(".hpp") ||
        filename.endswith(".hxx") || filename.endswith(".hh")) {
      const NamespaceDecl *targetNamespace = UDD->getNominatedNamespace();

      for (const Decl *decl : targetNamespace->getPrimaryContext()->decls()) {
        //check if using directive is other than class scope or function scope using declaration 
        if (isa<CXXRecordDecl>(decl) || isa<FunctionDecl>(decl)) {
    //if using directive other than class or function scope found then report error 
          DE.Report(UDD->getBeginLoc(), ID);
        }
      }
    }
  }


}

} // namespace clang::tidy::compliance
