set(LLVM_LINK_COMPONENTS 
    Support
)
add_clang_executable(M6-4-3
M6-4-3.cpp
)

target_link_libraries(M6-4-3
    PRIVATE
    clangAST
    clangASTMatchers
    clangBasic
    clangFrontend
    clangCodeGen
    clangTooling
)