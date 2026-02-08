add_library(ananas_ui STATIC
        AnanasLookAndFeel.cpp
)

target_include_directories(ananas_ui
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ananas_ui
        PUBLIC
        ananas_network_overview
        juce::juce_gui_basics
)
