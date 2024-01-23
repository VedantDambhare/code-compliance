
set(LLVM_LINK_COMPONENTS 
    Support
)
add_clang_executable(A6-1-6
A6-1-6.cpp
)

target_link_libraries(A6-1-6
    PRIVATE
    clangAST
    clangASTMatchers
    clangBasic
    clangFrontend
    clangCodeGen
    clangTooling
)