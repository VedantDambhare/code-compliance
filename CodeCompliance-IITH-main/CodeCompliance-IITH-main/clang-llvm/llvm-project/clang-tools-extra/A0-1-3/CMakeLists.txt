set(LLVM_LINK_COMPONENTS 
    Support
)
add_clang_executable(A0-1-3
detect_function_use.cpp
)

target_link_libraries(A0-1-3
    PRIVATE
    clangAST
    clangASTMatchers
    clangBasic
    clangFrontend
    clangCodeGen
    clangTooling
)