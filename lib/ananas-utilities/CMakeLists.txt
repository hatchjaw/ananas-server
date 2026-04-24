add_library(ananas_utilities INTERFACE)

set_target_properties(ananas_utilities PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        VERSION 0.3.0
        SOVERSION 1
)

target_include_directories(ananas_utilities
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
