
set(LLVM_LINK_COMPONENTS 
    Support
)
add_clang_executable(json_parser
json_parser.cpp
)

target_link_libraries(json_parser
    PRIVATE
    clangAST
    clangASTMatchers
    clangBasic
    clangFrontend
    clangCodeGen
    clangTooling
)