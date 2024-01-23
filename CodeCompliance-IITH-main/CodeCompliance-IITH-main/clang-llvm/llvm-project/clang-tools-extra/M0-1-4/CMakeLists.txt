set(LLVM_LINK_COMPONENTS 
    Support
)
add_clang_executable(M0-1-4
M0-1-4.cpp
)

target_link_libraries(M0-1-4
    PRIVATE
    clangAST
    clangASTMatchers
    clangBasic
    clangFrontend
    clangCodeGen
    clangTooling
)