set(LLVM_LINK_COMPONENTS 
    Support
)
add_clang_executable(direct_recursion
direct_recursion.cpp
)

target_link_libraries(direct_recursion
    PRIVATE
    clangAST
    clangASTMatchers
    clangBasic
    clangFrontend
    clangCodeGen
    clangTooling
)