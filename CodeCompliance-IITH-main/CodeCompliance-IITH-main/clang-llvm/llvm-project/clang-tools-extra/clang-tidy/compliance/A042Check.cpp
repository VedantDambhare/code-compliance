//===--- A042Check.cpp - clang-tidy ---------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

//include necessary header files 
#include "A042Check.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::compliance {

void A042Check::registerMatchers(MatchFinder *Finder) {
  // FIXME: Add matchers.
 //write matchers to match function,variable and class fields declarations 
  Finder -> addMatcher(functionDecl().bind("function"),this);
  Finder -> addMatcher(varDecl().bind("variable"), this);
  Finder -> addMatcher(fieldDecl().bind("field"), this);
}

void A042Check::check(const MatchFinder::MatchResult &Result) {
  // FIXME: Add callback implementation.
  //Initialize source manager and diagnostic engine 
  SourceManager &SM = Result.Context->getSourceManager();
  DiagnosticsEngine &DE = Result.Context->getDiagnostics();

    const unsigned ID = DE.getCustomDiagID(
    DiagnosticsEngine::Error, "AUTOSAR CPP Rule (A0.4.2) : Violated"); 

    //match function using function matcher 
     if (const FunctionDecl *FD = Result.Nodes.getNodeAs<FunctionDecl>("function")) {
       //check if the matched function is in main file and not header files
      if(SM.isWrittenInMainFile(SM.getFileLoc(FD->getBeginLoc())))
        //if return type is long double print error message 
        if(FD->getReturnType().getAsString()=="long double" || FD->getReturnType().getAsString()=="long double *"){
          DE.Report(FD->getBeginLoc(),ID);

        }
     }
    //match variable declarations 
     else if (const VarDecl *VD = Result.Nodes.getNodeAs<VarDecl>("variable") ){
        //check if the matched variable declaration is in main file and not header files
       if(SM.isWrittenInMainFile(SM.getFileLoc(VD->getBeginLoc()))){
         //if variable type is long double print error message 
            if( VD->getType().getAsString()=="long double" || VD->getType().getAsString()=="long double *"){
               DE.Report(VD->getBeginLoc(),ID);

            }
       }
    }
       //match field declarations of class and structs
    else if (const FieldDecl *FLD = Result.Nodes.getNodeAs<FieldDecl>("field") ){
       //check if the matched field declaration is in main file and not header files
       if(SM.isWrittenInMainFile(SM.getFileLoc(FLD->getBeginLoc()))){
          //if field declaration has type  long double print error message 
            if( FLD->getType().getAsString()=="long double" || FLD->getType().getAsString()=="long double *"){
               DE.Report(FLD->getBeginLoc(),ID);

            }
       }
    }
}

} // namespace clang::tidy::compliance
