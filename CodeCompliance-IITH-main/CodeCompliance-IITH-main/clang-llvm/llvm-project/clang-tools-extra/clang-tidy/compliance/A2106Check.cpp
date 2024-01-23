//===--- A2106Check.cpp - clang-tidy --------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//include the necessary header files 
#include "A2106Check.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::compliance {

void A2106Check::registerMatchers(MatchFinder *Finder) {
  // FIXME: Add matchers.
  //write a matcher to match classes and enums 
  Finder->addMatcher(namedDecl(anyOf(recordDecl(), enumDecl())).bind("namedDecl"), this);
}

void A2106Check::check(const MatchFinder::MatchResult &Result) {
  // FIXME: Add callback implementation.
  //Initialize diagnostic engine for error display 
  DiagnosticsEngine &DE = Result.Context->getDiagnostics();
  const unsigned ID = DE.getCustomDiagID(
      DiagnosticsEngine::Error,
      "AUTOSAR CPP Rule (A2.10.6) : Violated");
  //Use the matcher to match classes and enums 
  const NamedDecl *namedDecl = Result.Nodes.getNodeAs<NamedDecl>("namedDecl");
  std::string name = namedDecl->getNameAsString();
  SourceLocation declLoc = namedDecl->getLocation();
  //ignore if the matched class or enum is not in main file and in some header file
  if (!Result.SourceManager->isInMainFile(declLoc)) {
    return;
  }
  //iterate through all the matched declarations 
  for (auto *decl : namedDecl->getDeclContext()->decls()) {
    if (const VarDecl *varDecl = dyn_cast<VarDecl>(decl)) {
      //if declaration is variable and it matches with name of class and enum in same scope then print error
      if (name == varDecl->getNameAsString()) {
        DE.Report(varDecl->getBeginLoc(), ID);
      }
    }
    if (const FunctionDecl *fd = dyn_cast<FunctionDecl>(decl)) {
       //if declaration is function and it matches with name of class and enum in same scope then print error
      if (name == fd->getNameAsString()) {
        DE.Report(fd->getBeginLoc(), ID);
      }
    }
    if (const EnumDecl *enumDecl = dyn_cast<EnumDecl>(decl)) {
      std::string edname = enumDecl->getNameAsString();
       //if declaration is enum and it matches with name of class and enum or enum members in same scope then print error
      for (const EnumConstantDecl *enumConstant : enumDecl->enumerators()) {
        std::string ename = enumConstant->getNameAsString();
        if (name == ename or edname == ename) {
          DE.Report(enumConstant->getBeginLoc(), ID);
        }
      }
    }
  }
}

} // namespace clang::tidy::compliance
