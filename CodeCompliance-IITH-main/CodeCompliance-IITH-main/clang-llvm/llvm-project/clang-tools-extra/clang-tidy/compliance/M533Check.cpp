//===--- M533Check.cpp - clang-tidy ---------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "M533Check.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::compliance {

void M533Check::registerMatchers(MatchFinder *Finder) {
  // FIXME: Add matchers.
  Finder->addMatcher(cxxMethodDecl(hasName("operator&")).bind("method"), this);
}

void M533Check::check(const MatchFinder::MatchResult &Result) {
  // FIXME: Add callback implementation.
 SourceManager &SM = Result.Context->getSourceManager(); //for getting object locations
		DiagnosticsEngine &DE = Result.Context->getDiagnostics(); //for Error Display
		const unsigned ID = DE.getCustomDiagID( 
		DiagnosticsEngine ::Error, "AUTOSAR C++ Rule_M-5-3-3 violated.Operator & should not be overloaded ");
		// cout << "Checking ID: " << ID << '\n';
			
		if (const CXXMethodDecl *MD = Result.Nodes.getNodeAs<CXXMethodDecl>("method"))
		{
				if(MD->getDeclName().getAsString().rfind("operator&",0) == 0)
				{
					DE.Report(MD->getBeginLoc(),ID);
				}
		}
}

} // namespace clang::tidy::compliance
