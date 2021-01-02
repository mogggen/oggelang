project( compiler )

set( src_dir ${CMAKE_CURRENT_SOURCE_DIR}/src )

add_library( compiler 
    ${src_dir}/compiler.cpp
    ${src_dir}/block_alloc.cpp
    ${src_dir}/ast.cpp
    ${src_dir}/opcodes.cpp
    ${src_dir}/error.cpp
    ${src_dir}/lexer.cpp
    ${src_dir}/parser.cpp
    ${src_dir}/gen_bytecode.cpp
    ${src_dir}/linker.cpp
    ${src_dir}/debug_info.cpp
)

