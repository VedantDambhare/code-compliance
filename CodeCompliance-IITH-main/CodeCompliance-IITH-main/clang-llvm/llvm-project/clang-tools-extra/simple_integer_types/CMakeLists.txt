set(LLVM_LINK_COMPONENTS 
    Support
)
add_clang_executable(simple_integer_types
integer_parse.cpp
)

target_link_libraries(simple_integer_types
    PRIVATE
    clangAST
    clangASTMatchers
    clangBasic
    clangFrontend
    clangCodeGen
    clangTooling
)