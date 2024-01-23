//===--- A2104Check.cpp - clang-tidy --------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

//include necessary header files 
#include "A2104Check.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
using namespace std;
using namespace clang;
using namespace clang::tooling;
using namespace clang::ast_matchers;
using namespace llvm;

namespace clang::tidy::compliance {
//  pair hash function
template <class T1, class T2>
std::size_t pair_hash1(const std::pair<T1, T2>& p) {
    std::size_t seed = 0;
    boost::hash_combine(seed, p.first);
    boost::hash_combine(seed, p.second);
    return seed;
}
unordered_map<pair<string,string>,unsigned int,boost::hash<pair<string,string>>> var_namespace_map;

void A2104Check::registerMatchers(MatchFinder *Finder) {
  // FIXME: Add matchers.

  //write matcher to match variable declaration which is static and contained in a namespace 
  DeclarationMatcher VariableMatcher = varDecl(hasStaticStorageDuration(),hasAncestor(namespaceDecl())).bind("staticVar");
  Finder -> addMatcher(VariableMatcher, this);
}

void A2104Check::check(const MatchFinder::MatchResult &Result) {
  // FIXME: Add callback implementation.

  //initialize diagnostic engine 
  DiagnosticsEngine &DE = Result.Context->getDiagnostics();const unsigned ID = DE.getCustomDiagID(
  DiagnosticsEngine::Error, "AUTOSAR CPP Rule (A2.10.4) : Violated");
  //Use matcher to match variable declaration which is static and contained in a namespace 
  const VarDecl *VD = Result.Nodes.getNodeAs<VarDecl>("staticVar");
  if (VD && VD->getQualifiedNameAsString() != ""  && VD->isFileVarDecl() && VD->getStorageClass() ==clang::StorageClass::SC_Static){
    //get parent node of variable declaration (scope in which variable is declared)
      const DeclContext *Parent_of_var= VD->getDeclContext();
      if (const NamespaceDecl *NSD_for_var = dyn_cast<NamespaceDecl>(Parent_of_var)) {

        //make a pair of Variable and its corresponding namespace name 
      std::pair<std::string, std::string> key = std::make_pair(NSD_for_var->getQualifiedNameAsString(), VD->getNameAsString());
      if (var_namespace_map.find(key)==var_namespace_map.end()) {
        //incase the key is not found in map insert it with count 0
        var_namespace_map.insert({key,0});
      }
       if (var_namespace_map.find(key)!=var_namespace_map.end()) {
         //if found increment the count 
         var_namespace_map[key]++;
      }

      //if count of same static variable is greater than one for a single namespace (key) print error message 
      if (var_namespace_map[key] > 1) {
  
        DE.Report(VD->getBeginLoc(),ID);
      
      }


    }

}
}

} // namespace clang::tidy::compliance
