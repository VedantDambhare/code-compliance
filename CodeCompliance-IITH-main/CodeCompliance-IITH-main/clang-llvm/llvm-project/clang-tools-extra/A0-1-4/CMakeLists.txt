set(LLVM_LINK_COMPONENTS 
    Support
)
add_clang_executable(A0-1-4
unused_parameter.cpp
)

target_link_libraries(A0-1-4
    PRIVATE
    clangAST
    clangASTMatchers
    clangBasic
    clangFrontend
    clangCodeGen
    clangTooling
)