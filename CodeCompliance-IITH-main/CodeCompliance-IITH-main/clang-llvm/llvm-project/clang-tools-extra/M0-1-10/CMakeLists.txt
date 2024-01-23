set(LLVM_LINK_COMPONENTS 
    Support
)
add_clang_executable(M0-1-10
detect_unused_function.cpp
)

target_link_libraries(M0-1-10
    PRIVATE
    clangAST
    clangASTMatchers
    clangBasic
    clangFrontend
    clangCodeGen
    clangTooling
)