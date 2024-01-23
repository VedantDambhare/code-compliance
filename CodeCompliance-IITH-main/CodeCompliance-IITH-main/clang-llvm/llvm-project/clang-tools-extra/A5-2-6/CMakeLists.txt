set(LLVM_LINK_COMPONENTS 
    Support
)
add_clang_executable(A5-2-6
A5-2-6.cpp
)

target_link_libraries(A5-2-6
    PRIVATE
    clangAST
    clangASTMatchers
    clangBasic
    clangFrontend
    clangCodeGen
    clangTooling
)