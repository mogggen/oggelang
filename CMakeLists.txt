cmake_minimum_required( VERSION 3.8 )

project( oggelang )

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

set( include_dirs ${CMAKE_SOURCE_DIR}/compiler/src ${CMAKE_SOURCE_DIR}/interpreter/src )


add_subdirectory(compiler)
add_subdirectory(interpreter)

add_subdirectory(gui)
add_subdirectory(main)
