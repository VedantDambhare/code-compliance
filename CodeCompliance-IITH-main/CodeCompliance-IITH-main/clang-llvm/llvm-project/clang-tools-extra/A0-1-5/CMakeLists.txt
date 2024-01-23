set(LLVM_LINK_COMPONENTS 
    Support
)
add_clang_executable(A0-1-5
A0-1-5.cpp
)

target_link_libraries(A0-1-5
    PRIVATE
    clangAST
    clangASTMatchers
    clangBasic
    clangFrontend
    clangCodeGen
    clangTooling
)