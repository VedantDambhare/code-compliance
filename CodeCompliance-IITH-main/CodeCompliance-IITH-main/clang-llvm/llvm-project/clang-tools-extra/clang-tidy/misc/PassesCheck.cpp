//===--- PassesCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "PassesCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::misc {

struct pair_hash {
  template <class T1, class T2>
  std::size_t operator()(const std::pair<T1, T2> &p) const {
    std::size_t seed = 0;
    boost::hash_combine(seed, p.first);
    boost::hash_combine(seed, p.second);
    return seed;
  }
};
//  pair hash function
template <class T1, class T2>
std::size_t pair_hash1(const std::pair<T1, T2> &p) {
  std::size_t seed = 0;
  boost::hash_combine(seed, p.first);
  boost::hash_combine(seed, p.second);
  return seed;
}

void PassesCheck::registerMatchers(MatchFinder *Finder) {
  // FIXME: Add matchers.

  /*
    A0-4-2
  */
  Finder->addMatcher(functionDecl().bind("function"), this);
  Finder->addMatcher(varDecl().bind("variable"), this);
  Finder->addMatcher(fieldDecl().bind("field"), this);

  /*
    M6-4-2
  */
  Finder->addMatcher(ifStmt(hasAncestor(ifStmt())).bind("node"), this);

  /*
    A2-11-1
  */
  Finder->addMatcher(
      varDecl(hasType(isVolatileQualified())).bind("volatileVar"), this);
  
  Finder->addMatcher(fieldDecl(hasType(isVolatileQualified())).bind("field_vol"),
                     this);

  /*
    A7-2-4
  */
  Finder->addMatcher(enumDecl().bind("enumDecl"), this);

  /*
    A7-1-6
  */
  Finder->addMatcher(typedefDecl().bind("typedef"), this);

  /*
    A7-1-7
  */
  Finder->addMatcher(varDecl().bind("varDecl"), this);
  Finder->addMatcher(binaryOperator(hasOperatorName("=")).bind("assignment"),
                     this);
  Finder->addMatcher(
      unaryOperator(hasAnyOperatorName("++", "--")).bind("unaryOp"), this);
  Finder->addMatcher(
      binaryOperator(hasAnyOperatorName("-=", "+=", "*=", "/=", "%=", "&=",
                                        "|=", "^=", "<<=", ">>="))
          .bind("binOp"),
      this);

  /*
    M6-6-2
  */
  Finder->addMatcher(gotoStmt().bind("goto"), this);

  /*
    A7-4-1
  */
  Finder->addMatcher(asmStmt().bind("gccAsmStmt"), this);

  /*
    M7-3-2
  */
  Finder->addMatcher(functionDecl().bind("funcDecl"), this);

  /*
    A2-10-6
  */
  Finder->addMatcher(
      namedDecl(anyOf(recordDecl(), enumDecl())).bind("namedDecl"), this);

  /*
    A7-1-1
  */
  Finder->addMatcher(varDecl().bind("varDecl_1"), this);
  Finder->addMatcher(binaryOperator(hasOperatorName("=")).bind("assignment_1"),
                     this);
  Finder->addMatcher(
      unaryOperator(hasAnyOperatorName("++", "--")).bind("unaryOp_1"), this);
  Finder->addMatcher(
      binaryOperator(hasAnyOperatorName("-=", "+=", "*=", "/=", "%=", "&=",
                                        "|=", "^=", "<<=", ">>="))
          .bind("binOp_1"),
      this);

  /*
    M6-3-3
  */
  Finder->addMatcher(
      switchStmt(unless(hasDescendant(defaultStmt()))).bind("snode"), this);
  Finder->addMatcher(caseStmt(unless(hasDescendant(caseStmt()))).bind("cnode"),
                     this);

  /*
    A2-10-4
  */
  // Finder->addMatcher(
  //     varDecl(hasStaticStorageDuration(), hasAncestor(namespaceDecl()))
  //         .bind("staticVar"),
  //     this);

  /*
    M7-3-6
  */
  Finder->addMatcher(usingDirectiveDecl().bind("usingDirective"), this);

  /*
    M7-3-3
  */
  Finder -> addMatcher(namespaceDecl().bind("namespace"),this);
 


}

void PassesCheck::A0_4_2(const MatchFinder::MatchResult &Result) {

  DiagnosticsEngine &DE = Result.Context->getDiagnostics();

  

  unsigned ID = DE.getCustomDiagID(
      DiagnosticsEngine::Error,
      "AUTOSAR CPP Rule (A0.4.2) violated.Type long double shall not be used");

  if (const FunctionDecl *FD =
          Result.Nodes.getNodeAs<FunctionDecl>("function")) {
      
      if (!Result.SourceManager->isInMainFile(FD -> getBeginLoc())) {
        return;
      }
      if (FD->getReturnType().getAsString() == "long double" ||
          FD->getReturnType().getAsString() == "long double *") {
        DE.Report(FD->getBeginLoc(), ID);
      }
  }

  else if (const VarDecl *VD = Result.Nodes.getNodeAs<VarDecl>("variable")) {
      if (!Result.SourceManager->isInMainFile(VD -> getBeginLoc())) {
        return;
      }
      if (VD->getType().getAsString() == "long double" ||
          VD->getType().getAsString() == "long double *") {
        DE.Report(VD->getBeginLoc(), ID);
      }
    
  } else if (const FieldDecl *FLD =
                 Result.Nodes.getNodeAs<FieldDecl>("field")) {
      if (!Result.SourceManager->isInMainFile(FLD -> getBeginLoc())) {
        return;
      }
      if (FLD->getType().getAsString() == "long double" ||
          FLD->getType().getAsString() == "long double *") {
        DE.Report(FLD->getBeginLoc(), ID);
      }
    
  }
}

void PassesCheck::A2_11_1(const MatchFinder::MatchResult &Result) {
  
  DiagnosticsEngine &DE = Result.Context->getDiagnostics();

  unsigned ID = DE.getCustomDiagID(DiagnosticsEngine::Error,
                                   "AUTOSAR CPP Rule (A2.11.1) violated. "
                                   "Volatile keyword shall not be used");
  if (const VarDecl *Var = Result.Nodes.getNodeAs<VarDecl>("volatileVar")) {
    // outs() << "Found volatile variable: " << Var->getNameAsString() <<"\n";
    if (!Result.SourceManager->isInMainFile(Var -> getBeginLoc())) {
        return;
    }
    DE.Report(Var->getBeginLoc(), ID);
    

  } else if (const FieldDecl *FLD =
                 Result.Nodes.getNodeAs<FieldDecl>("field_vol")) {
      if (!Result.SourceManager->isInMainFile(FLD -> getBeginLoc())) {
        return;
      }
      DE.Report(FLD->getBeginLoc(), ID);
    
  }
}

void PassesCheck::M6_4_2(const MatchFinder::MatchResult &Result) {

  DiagnosticsEngine &DE = Result.Context->getDiagnostics();
  const clang::IfStmt *st = Result.Nodes.getNodeAs<clang::IfStmt>("node");
  
  if (!Result.SourceManager->isInMainFile(st -> getBeginLoc())) {
        return;
  }
  
  if (st->hasElseStorage() == false) {

    unsigned DiagID = DE.getCustomDiagID(
        DiagnosticsEngine::Error,
        "AUTOSAR C++ (Rule M6-4-2): Violated All if ... else if constructs "
        "shall be terminated with an else clause ");
    DE.Report(st->getBeginLoc(), DiagID);
  }
}

void PassesCheck::A7_1_6(const MatchFinder::MatchResult &Result) {

  DiagnosticsEngine &DE = Result.Context->getDiagnostics();
  const TypedefDecl *typedefDecl =
      Result.Nodes.getNodeAs<TypedefDecl>("typedef");
  SourceLocation loc = typedefDecl->getBeginLoc();
  if (!Result.SourceManager->isInMainFile(loc)) {
    return;
  }

  unsigned ID = DE.getCustomDiagID(
      DiagnosticsEngine::Error,
      "Rule A 7-1-6. The typedef specifier shall not be used.");
  DE.Report(loc, ID);
}

void PassesCheck::A7_2_4(const MatchFinder::MatchResult &Result) {

  DiagnosticsEngine &DE = Result.Context->getDiagnostics();
  const EnumDecl *enumDecl = Result.Nodes.getNodeAs<EnumDecl>("enumDecl");
  SourceLocation declLoc = enumDecl->getBeginLoc();
  if (!Result.SourceManager->isInMainFile(declLoc)) {
    return;
  }
  int i = 0;
  int count = 0;
  bool one = false;
  bool none = true;
  for (const EnumConstantDecl *enumConstant : enumDecl->enumerators()) {
    if (enumConstant->getInitExpr()) {
      none = false;
      if (i == 0) {
        one = true;
      } else {
        count++;
      }
    }
    i++;
  }
  if (none)
    return;
  if (one and count == 0)
    return;
  if (one and count == i - 1)
    return;

  unsigned ID = DE.getCustomDiagID(
      DiagnosticsEngine::Error,
      "Rule A 7-2-4. In an enumeration, either (1) none, (2) the first or (3) "
      "all enumerators shall be initialized.");
  DE.Report(enumDecl->getBeginLoc(), ID);
}

void PassesCheck::A7_4_1(const MatchFinder::MatchResult &Result) {

  DiagnosticsEngine &DE = Result.Context->getDiagnostics();
  const GCCAsmStmt *asmStmt = Result.Nodes.getNodeAs<GCCAsmStmt>("gccAsmStmt");
  if (!Result.SourceManager->isInMainFile(asmStmt -> getBeginLoc())) {
        return;
  }
  unsigned ID = DE.getCustomDiagID(
      DiagnosticsEngine::Error,
      "Rule A7-4-1 Violated. The asm declaration shall not be used.");

  DE.Report(asmStmt->getBeginLoc(), ID);
}

void PassesCheck::M7_3_2(const MatchFinder::MatchResult &Result) {
  const FunctionDecl *decl = Result.Nodes.getNodeAs<FunctionDecl>("funcDecl");
  SourceLocation declLoc = decl->getBeginLoc();
  if (!Result.SourceManager->isInMainFile(declLoc)) {
    return;
  }
  if (decl->getNameAsString() == "main") {
    i++;
  } else {
    return;
  }
  if (i > 1) {
    DiagnosticsEngine &Diags = Result.Context->getDiagnostics();
    unsigned ID = Diags.getCustomDiagID(
        DiagnosticsEngine::Error,
        "Rule M 7-3-2. The identifier main shall not be used for a function "
        "other than the global function main.");
    Diags.Report(decl->getLocation(), ID);
  }
}

void PassesCheck::A2_10_6(const MatchFinder::MatchResult &Result) {

  DiagnosticsEngine &DE = Result.Context->getDiagnostics();
  const unsigned ID = DE.getCustomDiagID(
      DiagnosticsEngine::Error,
      "AUTOSAR CPP Rule (A2.10.6) violated. A class or enumeration name shall "
      "not be hidden by a variable,function or enumerator declaration in the "
      "same scope.");
  const NamedDecl *namedDecl = Result.Nodes.getNodeAs<NamedDecl>("namedDecl");
  std::string name = namedDecl->getNameAsString();
  SourceLocation declLoc = namedDecl->getLocation();
  if (!Result.SourceManager->isInMainFile(declLoc)) {
    return;
  }
  for (auto *decl : namedDecl->getDeclContext()->decls()) {
    if (const VarDecl *varDecl = dyn_cast<VarDecl>(decl)) {
      if (name == varDecl->getNameAsString()) {
        DE.Report(varDecl->getBeginLoc(), ID);
      }
    }
    if (const FunctionDecl *fd = dyn_cast<FunctionDecl>(decl)) {
      if (name == fd->getNameAsString()) {
        DE.Report(fd->getBeginLoc(), ID);
      }
    }
    if (const EnumDecl *enumDecl = dyn_cast<EnumDecl>(decl)) {
      std::string edname = enumDecl->getNameAsString();
      for (const EnumConstantDecl *enumConstant : enumDecl->enumerators()) {
        std::string ename = enumConstant->getNameAsString();
        if (name == ename or edname == ename) {
          DE.Report(enumConstant->getBeginLoc(), ID);
        }
      }
    }
  }
}

void PassesCheck::A7_1_1(const MatchFinder::MatchResult &Result) {

  if (const VarDecl *varDecl = Result.Nodes.getNodeAs<VarDecl>("varDecl_1")) {
    SourceLocation loc = varDecl->getBeginLoc();
    if (!Result.SourceManager->isInMainFile(loc)) {
      return;
    }
    if (varDecl->isConstexpr() or varDecl->getType().isConstQualified()) {
      constVars.insert(varDecl->getNameAsString());
      return;
    }
    if (varDecl->getMostRecentDecl() == varDecl) {
      DiagnosticsEngine &DE = Result.Context->getDiagnostics();
      unsigned ID =
          DE.getCustomDiagID(DiagnosticsEngine::Error,
                             "Rule M 7-1-1. Constexpr or const specifiers "
                             "shall be used for immutable data declaration.");
      DE.Report(loc, ID);
    }
  }
  if (const BinaryOperator *binaryOperator =
          Result.Nodes.getNodeAs<BinaryOperator>("assignment_1")) {
    SourceLocation loc = binaryOperator->getBeginLoc();
    if (!Result.SourceManager->isInMainFile(loc)) {
      return;
    }
    const Expr *lhs = binaryOperator->getLHS();
    if (const DeclRefExpr *declRef = dyn_cast<DeclRefExpr>(lhs)) {
      if (constVars.count(declRef->getDecl()->getNameAsString())) {
        DiagnosticsEngine &DE = Result.Context->getDiagnostics();
        unsigned ID =
            DE.getCustomDiagID(DiagnosticsEngine::Error,
                               "Rule M 7-1-1. Constexpr or const specifiers "
                               "shall be used for immutable data declaration.");
        DE.Report(loc, ID);
      }
    }
  }
  if (const UnaryOperator *unaryOp =
          Result.Nodes.getNodeAs<UnaryOperator>("unaryOp_1")) {
    if (unaryOp->isIncrementDecrementOp() or
        unaryOp->getOpcode() == UO_PostInc or
        unaryOp->getOpcode() == UO_PostDec ||
        unaryOp->getOpcode() == UO_PreInc or
        unaryOp->getOpcode() == UO_PreDec) {
      SourceLocation loc = unaryOp->getOperatorLoc();
      if (loc.isValid()) {
        if (const DeclRefExpr *lhsVar =
                dyn_cast<DeclRefExpr>(unaryOp->getSubExpr())) {
          // Variable incremented or decremented
          if (constVars.count(lhsVar->getDecl()->getNameAsString())) {
            DiagnosticsEngine &DE = Result.Context->getDiagnostics();
            unsigned ID = DE.getCustomDiagID(
                DiagnosticsEngine::Error,
                "Rule M 7-1-1. Constexpr or const specifiers shall be used for "
                "immutable data declaration.");
            DE.Report(loc, ID);
          }
        }
      }
    }
  }

  if (const BinaryOperator *binOp =
          Result.Nodes.getNodeAs<BinaryOperator>("binOp_1")) {
    if (binOp->isCompoundAssignmentOp()) {
      SourceLocation loc = binOp->getBeginLoc();
      if (!Result.SourceManager->isInMainFile(loc)) {
        return;
      }
      const Expr *lhsExpr = binOp->getLHS()->IgnoreImpCasts();
      if (const DeclRefExpr *declRef = dyn_cast<DeclRefExpr>(lhsExpr)) {
        if (constVars.count(declRef->getDecl()->getNameAsString())) {
          // Variable on LHS of compound assignment
          DiagnosticsEngine &DE = Result.Context->getDiagnostics();
          unsigned ID = DE.getCustomDiagID(
              DiagnosticsEngine::Error,
              "Rule M 7-1-1. Constexpr or const specifiers shall be used for "
              "immutable data declaration.");
          DE.Report(loc, ID);
        }
      }
    }
  }
}

void PassesCheck::M6_3_3(const MatchFinder::MatchResult &Result) {

  DiagnosticsEngine &Diagnostics = Result.Context->getDiagnostics();
  if (const clang::CaseStmt *st =
          Result.Nodes.getNodeAs<clang::CaseStmt>("cnode")) {
    if (!Result.SourceManager->isInMainFile(st ->getBeginLoc())) {
        return;
    }
    clang::DynTypedNodeList NodeList = Result.Context->getParents(*st);
    int flag = 0;
    if (!NodeList.empty()) {
      clang::DynTypedNode ParentNode = NodeList[0];
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
                "AUTOSAR C++ (Rule M6-4-3): Violated Switch statement should "
                "be well formed switch statement ");
            Diagnostics.Report(st->getBeginLoc(), DiagID);
          }
          // If childStmt = st (our case stmt) flag = 1
          // If childStmt = Break and flag = 1: flag = 0
          // If childStmt = CaseStmt and flag = 1 raise error
        }
      }
    }
  } else if (const clang::SwitchStmt *st =
                 Result.Nodes.getNodeAs<clang::SwitchStmt>("snode")) {

    unsigned DiagID = Diagnostics.getCustomDiagID(
        DiagnosticsEngine::Error,
        "AUTOSAR C++ (Rule M6-4-3): Violated Switch statement should be well "
        "formed switch statement ");
    Diagnostics.Report(st->getBeginLoc(), DiagID);
  }
}

void PassesCheck::A2_10_4(const MatchFinder::MatchResult &Result) {

  DiagnosticsEngine &DE = Result.Context->getDiagnostics();
  const unsigned ID = DE.getCustomDiagID(
      DiagnosticsEngine::Error,
      "AUTOSAR CPP Rule (A2.10.4) violated. The identifier name of a "
      "non-member object with static storage duration or function shall not be "
      "reused within a namespace.");

  const VarDecl *VD = Result.Nodes.getNodeAs<VarDecl>("staticVar");
  if (!Result.SourceManager->isInMainFile(VD ->getBeginLoc())) {
        return;
  }
  if (VD && VD->getQualifiedNameAsString() != "" && VD->isFileVarDecl() &&
      VD->getStorageClass() == clang::StorageClass::SC_Static) {
    const DeclContext *Parent_of_var = VD->getDeclContext();
    if (const NamespaceDecl *NSD_for_var =
            dyn_cast<NamespaceDecl>(Parent_of_var)) {
      // outs() << "Parent Namespace: "
      // variable_namespace=NSD_for_var->getQualifiedNameAsString();

      std::pair<std::string, std::string> key = std::make_pair(
          NSD_for_var->getQualifiedNameAsString(), VD->getNameAsString());
      if (var_namespace_map.find(key) == var_namespace_map.end()) {
        // duplicate = VD->getBeginLoc();
        var_namespace_map.insert({key, 0});
      }
      if (var_namespace_map.find(key) != var_namespace_map.end()) {
        var_namespace_map[key]++;
      }
      //  outs()<<NSD_for_var->getQualifiedNameAsString()<<"\t"<<VD->getNameAsString()<<"\t"<<var_namespace_map[key]<<"\n";

      //  if(var_namespace_map.count({NSD_for_var->getQualifiedNameAsString(),VD->getNameAsString()})
      //  >1){

      //     DE.Report(VD->getBeginLoc(),ID);
      //  }

      //  std::size_t hash_value = pair_hash1(key);
      //  outs()<<hash_value<<"\n";

      if (var_namespace_map[key] > 1) {
        // DE.Report(duplicate,ID);
        DE.Report(VD->getBeginLoc(), ID);
        // var_namespace_map[key].second=VD->getBeginLoc();
      }
    }
    // const clang::FunctionDecl *FD
    // =Result.Nodes.getNodeAs<clang::FunctionDecl>("static_func");
  }
}

void PassesCheck::M7_3_6(const MatchFinder::MatchResult &Result) {
  DiagnosticsEngine &DE = Result.Context->getDiagnostics();
  const unsigned ID = DE.getCustomDiagID(
      DiagnosticsEngine::Error,
      "Rule M 7-3-6 violated. Using-directives and using-declarations "
      "(excluding class scope or function scope using-declarations) shall not "
      "be used in header files.");

  const UsingDirectiveDecl *UDD = Result.Nodes.getNodeAs<UsingDirectiveDecl>("usingDirective");
  if (!Result.SourceManager->isInMainFile(UDD ->getBeginLoc())) {
        return;
  }
  if (UDD) {

    SourceLocation loc = UDD->getBeginLoc();
    const FileEntry *fileEntry = Result.SourceManager->getFileEntryForID(
        Result.SourceManager->getFileID(loc));
    StringRef filename = fileEntry->getName();
    if (filename.endswith(".h") || filename.endswith(".hpp") ||
        filename.endswith(".hxx") || filename.endswith(".hh")) {
      const NamespaceDecl *targetNamespace = UDD->getNominatedNamespace();

      for (const Decl *decl : targetNamespace->getPrimaryContext()->decls()) {

        if (isa<CXXRecordDecl>(decl) || isa<FunctionDecl>(decl)) {

          DE.Report(UDD->getBeginLoc(), ID);
        }
      }
    }
  }
}

void PassesCheck::M7_3_3(const MatchFinder::MatchResult &Result) {
    DiagnosticsEngine &Diagnostics = Result.Context->getDiagnostics();
    
    if (const clang::NamespaceDecl *st = Result.Nodes.getNodeAs<clang::NamespaceDecl>("namespace")){
        if (!Result.SourceManager->isInMainFile(st ->getBeginLoc())) {
          return;
        }
        SourceLocation loc = st->getBeginLoc();
        const FileEntry *fileEntry = Result.SourceManager -> getFileEntryForID(Result.SourceManager -> getFileID(loc));
        StringRef filename = fileEntry->getName();
        if (filename.endswith(".h") || filename.endswith(".hpp") || filename.endswith(".hxx") || filename.endswith(".hh")) {
            
            if (st -> isAnonymousNamespace()) {
              unsigned DiagID = Diagnostics.getCustomDiagID(DiagnosticsEngine::Error, "AUTOSAR C++ (Rule M7-3-3): Violated There shall be no unnamed namespaces in header files.");
              Diagnostics.Report(st ->getBeginLoc(), DiagID);
            }
         }
        
    }

}

void PassesCheck::check(const MatchFinder::MatchResult &Result) {

  /*
    A0-4-2
  */
  //  A0_4_2(Result);

  // // /*
  // //   A2-11-1
  // // */
 
  //  A2_11_1(Result);

  /*
    M6-4-2
  */

  if (const clang::IfStmt *st = Result.Nodes.getNodeAs<clang::IfStmt>("node")) {
    M6_4_2(Result);
  }

  // /*
  //   A7-1-6
  // */

  if (const TypedefDecl *typedefDecl =
          Result.Nodes.getNodeAs<TypedefDecl>("typedef")) {
    A7_1_6(Result);
  }

  // /*
  //   A7-2-4
  // */

  if (const EnumDecl *enumDecl = Result.Nodes.getNodeAs<EnumDecl>("enumDecl")) {
    A7_2_4(Result);
  }

  // /*
  //   M7-3-6
  // */
  //M7_3_6(Result);
  

  // /*
  //   A7-1-7
  // */

  // /*
  //   M6-6-2
  // */

  // // const GotoStmt *gotoStmt = Result.Nodes.getNodeAs<GotoStmt>("goto");
  // // const LabelDecl *labelStmt = gotoStmt->getLabel();
  // // SourceLocation labelLoc = labelStmt->getBeginLoc();
  // // SourceLocation gotoLoc = gotoStmt->getGotoLoc();

  // // if (Result.SourceManager->isBeforeInTranslationUnit(labelLoc, gotoLoc)) {
  // //     DiagnosticsEngine &DE = Result.Context->getDiagnostics();
  // //     const unsigned ID = DE.getCustomDiagID(
  // //         DiagnosticsEngine::Error, "Rule M 6-6-2 violated. The goto
  // //         statement shall jump to a label declared later in the same function
  // //         body."
  // //         );
  // //     DE.Report(gotoLoc, ID) << labelStmt->getName();
  // // }

  // /*

  // */
  // // DiagnosticsEngine &DE = Result.Context->getDiagnostics();

  if (const GCCAsmStmt *asmStmt =
          Result.Nodes.getNodeAs<GCCAsmStmt>("gccAsmStmt")) {
    A7_4_1(Result);
  }

  if (const FunctionDecl *decl =
          Result.Nodes.getNodeAs<FunctionDecl>("funcDecl")) {
    M7_3_2(Result);
  }

  // /*
  //   A2-11-1
  // */
  // //A2_11_1(Result);

  // /*
  //   A2-10-6
  // */
  if (const NamedDecl *namedDecl = Result.Nodes.getNodeAs<NamedDecl>("namedDecl")) {
    A2_10_6(Result);
  }

  // /*
  //   M6-3-3
  // */
    M6_3_3(Result);


  // if (const VarDecl *VD = Result.Nodes.getNodeAs<VarDecl>("staticVar")) {
  //   A2_10_4(Result);    
  // }

  /*
       if (const clang::NamespaceDecl *st = Result.Nodes.getNodeAs<clang::NamespaceDecl>("namespace")){
    M7_3_3(Result);
  } 
  */

 
  


}

} // namespace clang::tidy::misc
