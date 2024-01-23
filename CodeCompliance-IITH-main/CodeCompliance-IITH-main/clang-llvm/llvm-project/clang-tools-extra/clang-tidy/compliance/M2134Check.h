//===--- M2134Check.h - clang-tidy ------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_COMPLIANCE_M2134CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_COMPLIANCE_M2134CHECK_H

#include "../ClangTidyCheck.h"

namespace clang::tidy::compliance {

/// FIXME: Write a short description.
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/compliance/M2-13-4.html
class M2134Check : public ClangTidyCheck {
public:
  M2134Check(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::compliance

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_COMPLIANCE_M2134CHECK_H
