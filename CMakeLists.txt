cmake_minimum_required( VERSION 3.8 )

project( ogge )

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

add_executable( ogge 
    src/main.cpp
    src/parser.cpp
    src/file_reader.cpp
)

set_property(TARGET ogge PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

