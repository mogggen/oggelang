cmake_minimum_required( VERSION 3.8 )

project( gui )

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

add_library( gui
    src/gui_main.cpp
    src/stb_image.cpp
    src/font.cpp
    src/window.cpp
    src/buttons.cpp
)

file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/deps/SDL2-2.0.12/lib/x64/SDL2.dll 
    DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )

set( SDL2_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps/SDL2-2.0.12/include )
set( FREETYPE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps/freetype-2.10.4/include )
include_directories( ${include_dirs} ${SDL2_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIR} )


set_property(TARGET gui PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
message(${CMAKE_SOURCE_DIR})

target_link_libraries( gui 
    ${CMAKE_CURRENT_SOURCE_DIR}/deps/SDL2-2.0.12/lib/x64/SDL2.lib 
    ${CMAKE_CURRENT_SOURCE_DIR}/deps/SDL2-2.0.12/lib/x64/SDL2main.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/deps/freetype-2.10.4/freetype.lib
)

