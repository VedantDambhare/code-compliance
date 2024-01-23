//===--- M013Check.cpp - clang-tidy ---------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//include necessary header files 
#include "M013Check.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::compliance {

void M013Check::registerMatchers(MatchFinder *Finder) {
  // FIXME: Add matchers.
  //write a matcher to match variable declaration which does not have ancestor as function or class 
  DeclarationMatcher VariableMatcher =
    varDecl(
            unless(hasAncestor(functionTemplateDecl())),
            unless(hasAncestor(classTemplateDecl())), 
            unless(isExpansionInSystemHeader()),  //System header ignored
            unless(parmVarDecl())).bind("variable");
  Finder -> addMatcher(VariableMatcher, this);
  Finder -> addMatcher(fieldDecl().bind("field"), this);
}

void M013Check::check(const MatchFinder::MatchResult &Result) {
  // FIXME: Add callback implementation.
  //use matcher to match fields of class or struct 
  if (const FieldDecl *FLD = Result.Nodes.getNodeAs<FieldDecl>("field") ){
      SourceLocation loc = FLD->getBeginLoc();
    //if match is found in file other than main file then ignore 
      if(!Result.SourceManager->isInMainFile(loc)) {
        return;
      }
    //check if matched field variable is used 
      if(FLD->isUsed() or FLD->isReferenced()) return;
    //if not used then report error 
      DiagnosticsEngine &DE = Result.Context->getDiagnostics();
      unsigned ID =
          DE.getCustomDiagID(DiagnosticsEngine::Error,
                            "MISRA C++ Rule 0-1-3 : Violation");
      DE.Report(loc, ID) << FLD->getNameAsString();
  }
    // Get the matched variable declaration node.
    const VarDecl *VD = Result.Nodes.getNodeAs<VarDecl>("variable");
    // Check if the variable declaration node is null, or if it is referenced or used.
    if (!VD || VD->isReferenced() || VD->isUsed()) {
      return;// Exit the function if the variable is referenced or used.
    }
    // Get the diagnostic engine.
    DiagnosticsEngine &DE = Result.Context->getDiagnostics();
    // Create a custom diagnostic message.
    unsigned ID =
        DE.getCustomDiagID(DiagnosticsEngine::Error,
                           "AUTOSAR C++ Rule 0-1-3 : Violation");
    
    
    // Get the location of the variable declaration and its source range.
    SourceLocation loc = VD->getLocation();
    //SourceRange range = VD->getSourceRange();
    // Report the diagnostic message.
    DE.Report(loc, ID) << VD->getNameAsString();
}

} // namespace clang::tidy::compliance
