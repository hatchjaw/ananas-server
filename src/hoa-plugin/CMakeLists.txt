set(companyName "Emeraude")

set(companyCode "EMRD")

set(pluginCode Ahoa)

set(bundleID "org.emeraude.ananas-hoa")

set(juceFormats Standalone)

juce_add_plugin(ananasHOA
        COMPANY_NAME "${companyName}"
        IS_SYNTH FALSE
        NEEDS_MIDI_INPUT FALSE
        COPY_PLUGIN_AFTER_BUILD TRUE
        PLUGIN_MANUFACTURER_CODE "${companyCode}"
        PLUGIN_CODE ${pluginCode}
        FORMATS ${juceFormats}
        PRODUCT_NAME "AnanasHOA"
)

clap_juce_extensions_plugin(
        TARGET ananasHOA
        CLAP_ID "${bundleID}"
        CLAP_FEATURES utility
)

target_compile_definitions(ananasHOA
        PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        SHOW_NO_NETWORK_OVERLAY=$<BOOL:${SHOW_NO_NETWORK_OVERLAY}>
)

target_sources(ananasHOA
        PRIVATE
        PluginEditor.cpp
        PluginProcessor.cpp
        UI/HoaLookAndFeel.cpp
        UI/HOAInterfaceComponent.cpp
        UI/SoundFieldComponent.cpp
        UI/ModuleComponent.cpp
        UI/SpeakerIconComponent.cpp
)

target_link_libraries(ananasHOA
        PRIVATE
        juce::juce_core
        juce::juce_osc
        juce::juce_audio_devices
        juce::juce_audio_utils
        clap_juce_extensions
        ananas_utilities
        ananas_ui
        ananas_server
        ananas_network_overview
        PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
