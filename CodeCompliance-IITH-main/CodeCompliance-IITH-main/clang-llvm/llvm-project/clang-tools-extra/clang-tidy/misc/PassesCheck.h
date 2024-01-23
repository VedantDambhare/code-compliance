//===--- PassesCheck.h - clang-tidy -----------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISC_PASSESCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISC_PASSESCHECK_H

#include "../ClangTidyCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include <boost/functional/hash.hpp>

using namespace clang::ast_matchers;
namespace clang::tidy::misc {

/// FIXME: Write a short description.
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misc/passes.html
class PassesCheck : public ClangTidyCheck {
public:
  PassesCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  void A0_4_2(const MatchFinder::MatchResult &Result);
  void A2_11_1(const MatchFinder::MatchResult &Result);
  void M6_4_2(const MatchFinder::MatchResult &Result);
  void A7_1_6(const MatchFinder::MatchResult &Result);
  void A7_2_4(const MatchFinder::MatchResult &Result);
  void A7_4_1(const MatchFinder::MatchResult &Result);
  void M7_3_2(const MatchFinder::MatchResult &Result);
  void A2_10_6(const MatchFinder::MatchResult &Result);
  void A7_1_1(const MatchFinder::MatchResult &Result);
  void M6_3_3(const MatchFinder::MatchResult &Result);
  void A2_10_4(const MatchFinder::MatchResult &Result);
  void M7_3_6(const MatchFinder::MatchResult &Result);
  void M7_3_3(const MatchFinder::MatchResult &Result);
private:
  int i = 0;
  std::set<std::string> constVars;
  unsigned int count = 0;
  std::unordered_map<std::pair<std::string,std::string>,unsigned int,boost::hash<std::pair<std::string,std::string>>> var_namespace_map;
};

} // namespace clang::tidy::misc

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISC_PASSESCHECK_H
