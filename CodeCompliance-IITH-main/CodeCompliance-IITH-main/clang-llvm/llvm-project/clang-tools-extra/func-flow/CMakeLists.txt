set(LLVM_LINK_COMPONENTS 
    Support
)
add_clang_executable(func-flow
    flow.cpp
)

target_link_libraries(func-flow
    PRIVATE
    clangAST
    clangASTMatchers
    clangBasic
    clangFrontend
    clangCodeGen
    clangTooling
)