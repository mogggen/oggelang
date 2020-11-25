project( ogge )

add_executable( ogge
    src/main.cpp        
)

include_directories( ogge ${include_dirs} ${CMAKE_SOURCE_DIR}/gui/src )
include_directories( ${include_dirs} ${SDL2_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIR} )

set_property(TARGET ogge PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
target_link_libraries( ogge compiler interpreter gui )
