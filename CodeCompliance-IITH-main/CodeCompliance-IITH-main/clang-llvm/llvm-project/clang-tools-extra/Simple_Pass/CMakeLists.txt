set(LLVM_LINK_COMPONENTS 
    Support
)
add_clang_executable(Simple_Pass
Simple_Pass.cpp
)

target_link_libraries(Simple_Pass
    PRIVATE
    clangAST
    clangASTMatchers
    clangBasic
    clangFrontend
    clangCodeGen
    clangTooling
)