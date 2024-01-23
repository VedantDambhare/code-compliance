//===--- A711Check.cpp - clang-tidy ---------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//include necessary header files 
#include "A711Check.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::compliance {

void A711Check::registerMatchers(MatchFinder *Finder) {
  // FIXME: Add matchers.
  //write matcher to match variable declarations, unary and binary operators 
  Finder -> addMatcher(varDecl().bind("varDecl"), this);
  Finder -> addMatcher(binaryOperator(hasOperatorName("=")).bind("assignment"), this);
  Finder -> addMatcher(unaryOperator(hasAnyOperatorName("++", "--")).bind("unaryOp"), this);
  Finder -> addMatcher(binaryOperator(hasAnyOperatorName("-=", "+=", "*=", "/=", "%=", "&=", "|=", "^=", "<<=", ">>=")).bind("binOp"), this);

}

void A711Check::check(const MatchFinder::MatchResult &Result) {
  // FIXME: Add callback implementation.
  //Use variable declaration matcher 
  if (const VarDecl *varDecl = Result.Nodes.getNodeAs<VarDecl>("varDecl")) {
      SourceLocation loc = varDecl->getBeginLoc();
    //if matched variable is not in main file then ignore 
      if (!Result.SourceManager->isInMainFile(loc)) {
        return;
      }
    //check if variable has const specifier 
      if(varDecl->isConstexpr() or varDecl->getType().isConstQualified()){
        //insert it into list of const vars incase it has const specifier 
        constVars.insert(varDecl->getNameAsString());
        return;
      }
    //check for redeclaration of the matched variable and report error incase match found 
      if(varDecl->getMostRecentDecl() == varDecl){
        DiagnosticsEngine &DE = Result.Context->getDiagnostics();
        unsigned ID = DE.getCustomDiagID(
            DiagnosticsEngine::Error, "AUTOSAR C++ Rule M 7-1-1 : Violated");
        DE.Report(loc, ID);
      }
    }
  //match binary operator '='
    if(const BinaryOperator *binaryOperator = Result.Nodes.getNodeAs<BinaryOperator>("assignment")){
      SourceLocation loc = binaryOperator->getBeginLoc();
      //if the match is not found in main file then ignore 
      if (!Result.SourceManager->isInMainFile(loc)) {
        return;
      }
      //get left hand side variable of matched binary operator '=' 
      const Expr *lhs = binaryOperator->getLHS();
      //if variable is found on lhs it means its modified. Check if its present in const variables list ad if found report error 
      if (const DeclRefExpr *declRef = dyn_cast<DeclRefExpr>(lhs)) {
          if(constVars.count(declRef->getDecl()->getNameAsString())){
            //initialize diagnostic engine to report error
            DiagnosticsEngine &DE = Result.Context->getDiagnostics();
            unsigned ID = DE.getCustomDiagID(
                DiagnosticsEngine::Error, "AUTOSAR C++ Rule M 7-1-1 : Violated");
            DE.Report(loc, ID);
          }
      }
    }

  //use m,atcher to match unary operators 
    if(const UnaryOperator *unaryOp = Result.Nodes.getNodeAs<UnaryOperator>("unaryOp")){
      //check if unary operator is modified using increment or decrement operators 
      if (unaryOp->isIncrementDecrementOp() or unaryOp->getOpcode() == UO_PostInc or unaryOp->getOpcode() == UO_PostDec || unaryOp->getOpcode() == UO_PreInc or unaryOp->getOpcode() == UO_PreDec) {
      SourceLocation loc = unaryOp->getOperatorLoc();
        //incase unary operator is modified and is in const var list then report error 
      if (loc.isValid()) {
        if (const DeclRefExpr *lhsVar = dyn_cast<DeclRefExpr>(unaryOp->getSubExpr())) {
          //Variable incremented or decremented
          if(constVars.count(lhsVar->getDecl()->getNameAsString())){
            DiagnosticsEngine &DE = Result.Context->getDiagnostics();
            unsigned ID = DE.getCustomDiagID(
                DiagnosticsEngine::Error, "AUTOSAR C++ Rule M 7-1-1 : Violated");
            DE.Report(loc, ID);
          }
        }
        }
      }
    }
    if(const BinaryOperator *binOp = Result.Nodes.getNodeAs<BinaryOperator>("binOp")){
      if (binOp->isCompoundAssignmentOp()) {
        SourceLocation loc = binOp->getBeginLoc();
        if (!Result.SourceManager->isInMainFile(loc)) {
          return;
        }
      const Expr *lhsExpr = binOp->getLHS()->IgnoreImpCasts();
      if (const DeclRefExpr *declRef = dyn_cast<DeclRefExpr>(lhsExpr)) {
        if(constVars.count(declRef->getDecl()->getNameAsString())){
           //Variable on LHS of compound assignment
            DiagnosticsEngine &DE = Result.Context->getDiagnostics();
            unsigned ID = DE.getCustomDiagID(
                DiagnosticsEngine::Error, "AUTOSAR C++ Rule M 7-1-1 : Violated");
            DE.Report(loc, ID);
        }
      }
    }
    }

}

} // namespace clang::tidy::compliance
