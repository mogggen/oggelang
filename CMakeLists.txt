cmake_minimum_required( VERSION 3.8 )

project( ogge )

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

set( src_dir ${CMAKE_CURRENT_SOURCE_DIR}/src )

add_executable( ogge 
    ${src_dir}/main.cpp
    ${src_dir}/block_alloc.cpp
    ${src_dir}/ast.cpp
    ${src_dir}/error.cpp
    ${src_dir}/lexer.cpp
    ${src_dir}/parser.cpp
    ${src_dir}/compiler.cpp
    ${src_dir}/interpreter.cpp
)

set_property(TARGET ogge PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

