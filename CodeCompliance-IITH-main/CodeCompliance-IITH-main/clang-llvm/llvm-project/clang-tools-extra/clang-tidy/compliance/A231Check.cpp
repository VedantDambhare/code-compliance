//===--- A231Check.cpp - clang-tidy ---------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "A231Check.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::compliance {

void A231Check::registerMatchers(MatchFinder *Finder) {
  // FIXME: Add matchers.
  Finder->addMatcher(characterLiteral().bind("charLiteral"), this);
}

void A231Check::check(const MatchFinder::MatchResult &Result) {
  // FIXME: Add callback implementation.
  DiagnosticsEngine &D = Result.Context->getDiagnostics();
       //  const CharacterLiteral *charLiteral = Result.Nodes.getNodeAs<CharacterLiteral>("charLiteral");
         //      llvm::outs() << ""<< charLiteral->getValue() << "\n";

        if (const CharacterLiteral *charLiteral = Result.Nodes.getNodeAs<CharacterLiteral>("charLiteral")) { // this is the character literal
            
            const unsigned int charValue = charLiteral->getValue(); // this is the value of the character literal
        
              
            // i need to check for differen types of character literals here
            if (charValue > 127 ) { //Sheet provided have only describes that the ASCII valid  are till 127 , checked border no need to give 0 
              
                const unsigned ID = D.getCustomDiagID(DiagnosticsEngine::Warning, "Violation of rule A2-3-1: Reused static variable name within the namespace: %0");
                D.Report(charLiteral->getBeginLoc(), ID) << charValue;
                //
             //  llvm::outs() << "Invalid character found at line " << Result.SourceManager->getSpellingLineNumber(charLiteral->getBeginLoc())
               //        << ": " << charValue << "\n";
            }
        }
}

} // namespace clang::tidy::compliance
