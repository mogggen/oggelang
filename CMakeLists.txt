cmake_minimum_required( VERSION 3.8 )

project( oggelang )

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

set( include_dirs ${CMAKE_SOURCE_DIR}/compiler/src ${CMAKE_SOURCE_DIR}/interpreter/src )

set( SDL2_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/gui/deps/SDL2-2.0.12/include )
set( FREETYPE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/gui/deps/freetype-2.10.4/include )

add_subdirectory(compiler)
add_subdirectory(interpreter)

add_subdirectory(main)
