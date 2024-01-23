//===--- M5210Check.cpp - clang-tidy --------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//include necessary header files 
#include "M5210Check.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::compliance {

void M5210Check::registerMatchers(MatchFinder *Finder) {
  // FIXME: Add matchers.
  //write matcher to match binary operator 
  Finder -> addMatcher(binaryOperator().bind("binaryOp"), this);
}

void M5210Check::check(const MatchFinder::MatchResult &Result) {
  // FIXME: Add callback implementation.
  //use matcher to match binary operator 
  if (const BinaryOperator *binaryOp = Result.Nodes.getNodeAs<BinaryOperator>("binaryOp")) {
        SourceLocation loc = binaryOp->getBeginLoc();
    //if match is found in file other than main file then ignore 
        if (!Result.SourceManager->isInMainFile(loc)) {
            return;
        }
        //get lhs and rhs variables of binary operator 
        const Expr *lhsExpr = binaryOp->getLHS();
        const Expr *rhsExpr = binaryOp->getRHS();
        bool flag = false;
    //check if unary operatos are used with lhs of binary operator (mixing detected). Set the error flag
        if (const UnaryOperator *unaryOp = dyn_cast<UnaryOperator>(lhsExpr)) {
            flag = unaryOp->getOpcode() == UO_PreInc || unaryOp->getOpcode() == UO_PreDec ||
                    unaryOp->getOpcode() == UO_PostInc || unaryOp->getOpcode() == UO_PostDec;
        }
    //check if unary operatos are used with rhs  of binary operator (mixing detected) .Set the error flag 
        if (const UnaryOperator *unaryOp = dyn_cast<UnaryOperator>(rhsExpr)) {
            flag = unaryOp->getOpcode() == UO_PreInc || unaryOp->getOpcode() == UO_PreDec ||
                    unaryOp->getOpcode() == UO_PostInc || unaryOp->getOpcode() == UO_PostDec;
        }
    //if error flag is set the print error message 
        if (flag) {
          DiagnosticsEngine &DE = Result.Context->getDiagnostics();
            unsigned ID = DE.getCustomDiagID(
                DiagnosticsEngine::Error, "AUTOSAR C++ Rule M 5-2-10 (Advisory) : Violated ");
            DE.Report(loc, ID);
        }
      }
  } 

} // namespace clang::tidy::compliance
