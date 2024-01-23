set(LLVM_LINK_COMPONENTS 
    Support
)
add_clang_executable(M6-2-1
M6-2-1.cpp
)

target_link_libraries(M6-2-1
    PRIVATE
    clangAST
    clangASTMatchers
    clangBasic
    clangFrontend
    clangCodeGen
    clangTooling
)