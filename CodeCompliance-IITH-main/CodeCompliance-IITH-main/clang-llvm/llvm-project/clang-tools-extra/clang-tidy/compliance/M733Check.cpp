//===--- M733Check.cpp - clang-tidy ---------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//include necessary header files 
#include "M733Check.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::compliance {

void M733Check::registerMatchers(MatchFinder *Finder) {
  // FIXME: Add matchers.
  //write matcher to match namespace nodes 
  Finder -> addMatcher(namespaceDecl().bind("namespace"),this);
}

void M733Check::check(const MatchFinder::MatchResult &Result) {
  // FIXME: Add callback implementation.
  //initialize diagnostic engine 
   DiagnosticsEngine &Diagnostics = Result.Context->getDiagnostics();
    //Use namespace matcher to match namespace declarations 
    if (const clang::NamespaceDecl *st = Result.Nodes.getNodeAs<clang::NamespaceDecl>("namespace")){
      //if match is foyund outside main file in some header file then ignore 
        if (!Result.SourceManager->isInMainFile(st ->getBeginLoc())) {
          return;
        }
        SourceLocation loc = st->getBeginLoc();
      //get extension of main file 
        const FileEntry *fileEntry = Result.SourceManager -> getFileEntryForID(Result.SourceManager -> getFileID(loc));
        StringRef filename = fileEntry->getName();
      //check if main file is header file 
        if (filename.endswith(".h") || filename.endswith(".hpp") || filename.endswith(".hxx") || filename.endswith(".hh")) {
            //if namespace without name is found then report error 
            if (st -> isAnonymousNamespace()) {
              unsigned DiagID = Diagnostics.getCustomDiagID(DiagnosticsEngine::Error, "AUTOSAR C++ (Rule M7-3-3): Violated ");
              Diagnostics.Report(st ->getBeginLoc(), DiagID);
            }
         }
        
    }
}

} // namespace clang::tidy::compliance
