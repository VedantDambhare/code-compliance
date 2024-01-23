//===--- M643Check.cpp - clang-tidy ---------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

//include necessary header files 
#include "M643Check.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::compliance {

void M643Check::registerMatchers(MatchFinder *Finder) {
  // FIXME: Add matchers.
  //write matcher to match switch statement which does not have default case 
  Finder->addMatcher(
      switchStmt(unless(hasDescendant(defaultStmt()))).bind("snode"), this);
  //write matcher to match case statement which does not have case statement as descendent in  AST 
  Finder->addMatcher(caseStmt(unless(hasDescendant(caseStmt()))).bind("cnode"),
                     this);
}

void M643Check::check(const MatchFinder::MatchResult &Result) {
  // FIXME: Add callback implementation.
  //initialize diagnostic engine 
  DiagnosticsEngine &Diagnostics = Result.Context->getDiagnostics();
  //use case statement matcher 
  if (const clang::CaseStmt *st =
          Result.Nodes.getNodeAs<clang::CaseStmt>("cnode")) {
    //if match is not found in main file then ignore 
    if (!Result.SourceManager->isInMainFile(st ->getBeginLoc())) {
        return;
    }
    //get parent node of case statement 
    clang::DynTypedNodeList NodeList = Result.Context->getParents(*st);
    int flag = 0;
    if (!NodeList.empty()) {
      clang::DynTypedNode ParentNode = NodeList[0];
      //check if paremt node of case has compound statements (case is not empty) 
      if (const CompoundStmt *Parent = ParentNode.get<CompoundStmt>()) {
        for (const Stmt *childStmt : Parent->children()) {
          if (childStmt == st) {
            flag = 1;
          } else if (isa<BreakStmt>(childStmt) && flag == 1) {
            flag = 0;
            break;
          } else if (isa<CaseStmt>(childStmt) && flag == 1) {
            unsigned DiagID = Diagnostics.getCustomDiagID(
                DiagnosticsEngine::Error,
                "AUTOSAR C++ (Rule M6-4-3): Violated");
            Diagnostics.Report(st->getBeginLoc(), DiagID);
          }
        }
      }
    }
  } else if (const clang::SwitchStmt *st =
                 Result.Nodes.getNodeAs<clang::SwitchStmt>("snode")) {

    unsigned DiagID = Diagnostics.getCustomDiagID(
        DiagnosticsEngine::Error,
        "AUTOSAR C++ (Rule M6-4-3): Violated ");
    Diagnostics.Report(st->getBeginLoc(), DiagID);
  }
}

} // namespace clang::tidy::compliance
