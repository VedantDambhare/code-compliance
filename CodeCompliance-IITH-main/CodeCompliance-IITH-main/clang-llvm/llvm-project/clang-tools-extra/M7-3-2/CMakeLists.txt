set(LLVM_LINK_COMPONENTS 
    Support
)
add_clang_executable(M7-3-2
M7-3-2.cpp
)

target_link_libraries(M7-3-2
    PRIVATE
    clangAST
    clangASTMatchers
    clangBasic
    clangFrontend
    clangCodeGen
    clangTooling
)