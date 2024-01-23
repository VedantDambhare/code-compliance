//===------- BoostTidyModule.cpp - clang-tidy -----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "../ClangTidy.h"
#include "../ClangTidyModule.h"
#include "../ClangTidyModuleRegistry.h"
#include "A012Check.h"
#include "A042Check.h"
#include "A042Check.h"
#include "A2104Check.h"
#include "A2106Check.h"
#include "A2111Check.h"
#include "A231Check.h"
#include "A521Check.h"
#include "A523Check.h"
#include "A711Check.h"
#include "A716Check.h"
#include "A722Check.h"
#include "A723Check.h"
#include "A724Check.h"
#include "A741Check.h"
#include "M013Check.h"
#include "M2133Check.h"
#include "M2134Check.h"
#include "M505Check.h"
#include "M5210Check.h"
#include "M5212Check.h"
#include "M531Check.h"
#include "M532Check.h"
#include "M533Check.h"
#include "M621Check.h"
#include "M642Check.h"
#include "M643Check.h"
#include "M647Check.h"
#include "M662Check.h"
#include "M732Check.h"
#include "M733Check.h"
#include "M736Check.h"

using namespace clang::ast_matchers;

namespace clang::tidy {
namespace compliance {

class ComplianceModule : public ClangTidyModule {
public:
  void addCheckFactories(ClangTidyCheckFactories &CheckFactories) override {
    
    CheckFactories.registerCheck<A012Check>(
        "compliance-A0-1-2");
    CheckFactories.registerCheck<A042Check>(
        "compliance-A0-4-2");
    CheckFactories.registerCheck<A2104Check>(
        "compliance-A2-10-4");
    CheckFactories.registerCheck<A2106Check>(
        "compliance-A2-10-6");
    CheckFactories.registerCheck<A2111Check>(
        "compliance-A2-11-1");
    CheckFactories.registerCheck<A231Check>(
        "compliance-A2-3-1");
    CheckFactories.registerCheck<A521Check>(
        "compliance-A5-2-1");
    CheckFactories.registerCheck<A523Check>(
        "compliance-A5-2-3");
    CheckFactories.registerCheck<A711Check>(
        "compliance-A7-1-1");
    CheckFactories.registerCheck<A716Check>(
        "compliance-A7-1-6");
    CheckFactories.registerCheck<A722Check>(
        "compliance-A7-2-2");
    CheckFactories.registerCheck<A723Check>(
        "compliance-A7-2-3");
    CheckFactories.registerCheck<A724Check>(
        "compliance-A7-2-4");
    CheckFactories.registerCheck<A741Check>(
        "compliance-A7-4-1");
    CheckFactories.registerCheck<M013Check>(
        "compliance-M0-1-3");
    // CheckFactories.registerCheck<M2133Check>(
    //     "compliance-M2-13-3");
    // CheckFactories.registerCheck<M2134Check>(
    //     "compliance-M2-13-4");
    CheckFactories.registerCheck<M505Check>(
        "compliance-M5-0-5");
    CheckFactories.registerCheck<M5210Check>(
        "compliance-M5-2-10");
    CheckFactories.registerCheck<M5212Check>(
        "compliance-M5-2-12");
    CheckFactories.registerCheck<M531Check>(
        "compliance-M5-3-1");
    CheckFactories.registerCheck<M532Check>(
        "compliance-M5-3-2");
    CheckFactories.registerCheck<M533Check>(
        "compliance-M5-3-3");
    CheckFactories.registerCheck<M621Check>(
        "compliance-M6-2-1");
    CheckFactories.registerCheck<M642Check>(
        "compliance-M6-4-2");
    CheckFactories.registerCheck<M643Check>(
        "compliance-M6-4-3");
    CheckFactories.registerCheck<M647Check>(
        "compliance-M6-4-7");
    CheckFactories.registerCheck<M662Check>(
        "compliance-M6-6-2");
    CheckFactories.registerCheck<M732Check>(
        "compliance-M7-3-2");
    CheckFactories.registerCheck<M733Check>(
        "compliance-M7-3-3");
    CheckFactories.registerCheck<M736Check>(
        "compliance-M7-3-6");
  }
};

} // namespace boost

// Register the BoostModule using this statically initialized variable.
static ClangTidyModuleRegistry::Add<compliance::ComplianceModule> X("compliance-module",
                                                   "Add compliance checks.");

// This anchor is used to force the linker to link in the generated object file
// and thus register the BoostModule.
volatile int ComplianceModuleAnchorSource = 0;

} // namespace clang::tidy
