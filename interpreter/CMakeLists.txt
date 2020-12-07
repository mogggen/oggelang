project( interpreter )

set( src_dir ${CMAKE_CURRENT_SOURCE_DIR}/src )

include_directories( ${include_dirs} )

add_library( interpreter 
    ${src_dir}/interpreter.cpp
    ${src_dir}/debugger.cpp
)
