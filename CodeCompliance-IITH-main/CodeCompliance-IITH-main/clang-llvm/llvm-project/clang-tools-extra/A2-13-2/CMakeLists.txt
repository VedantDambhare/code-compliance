
set(LLVM_LINK_COMPONENTS 
    Support
)
add_clang_executable(A2-13-2
A2-13-2.cpp
)

target_link_libraries(A2-13-2
    PRIVATE
    clangAST
    clangASTMatchers
    clangBasic
    clangFrontend
    clangCodeGen
    clangTooling
)