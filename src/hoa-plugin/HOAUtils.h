#ifndef HOAUTILS_H
#define HOAUTILS_H

#include <juce_core/juce_core.h>

namespace ananas::HOA
{
    enum CommandIDs
    {
        SwitchToHoaTab = 0x1001,
        SwitchToNetworkTab = 0x1002,
        RevealModuleSelector = 0x1003
    };

    class Constants
    {
    public:
        constexpr static size_t NumChannels{16};
        constexpr static size_t AmbisonicsOrder{3};
        constexpr static size_t NumSpeakers{16};
    };

    enum class SpeakerCoordinateAxis : char
    {
        X = 'x',
        Y = 'y',
        Z = 'z'
    };

    class Params
    {
    public:
        struct Param
        {
            juce::StringRef id;
            juce::StringRef name;
        };

        struct RangedParamInt : Param
        {
            int minValue{0};
            int maxValue{0};
            int defaultValue{0};
        };

        struct RangedParamFloat : Param
        {
            juce::NormalisableRange<float> range;
            juce::NormalisableRange<double> rangeDouble;
            float defaultValue{0.f};
        };

        struct BoolParam : Param
        {
            bool defaultValue{true};
        };

        static juce::String getSpeakerCoordinateParamID(const size_t index, SpeakerCoordinateAxis axis)
        {
            return "speaker" + juce::String{index} + static_cast<char>(axis);
        }

        static juce::String getSpeakerCoordinateParamName(const size_t index, SpeakerCoordinateAxis axis)
        {
            return "Speaker " + juce::String{index} + " " + static_cast<char>(axis);
        }

        static juce::String getDecoderCoefficientParamID(const int row, const size_t index)
        {
            return "/dm/" + juce::String{row} + "/" + juce::String{index};
        }

        static juce::String getDecoderRowParamID(const int row)
        {
            return "/row/" + juce::String{row};
        }

        inline static const juce::NormalisableRange<float> SpeakerCoordinateParamRange{
            -10.f, 10.f
        };

        static constexpr float SpeakerCoordinateParamDefault{0.f};
    };

    class Strings
    {
    public:
        inline static const juce::StringRef HoaTabName{"HOA Control"};
        inline static const juce::StringRef NetworkTabName{"Network Overview"};

        inline static const juce::StringRef TabsCommandCategoryName{"Tabs"};
        inline static const juce::StringRef SwitchToHoaTabShortName{"HOA Control tab"};
        inline static const juce::StringRef SwitchToHoaTabDescription{"Switches to the HOA Control tab"};
        inline static const juce::StringRef SwitchToNetworkTabShortName{"Network overview tab"};
        inline static const juce::StringRef SwitchToNetworkTabDescription{"Switches to the Network Overview tab"};

        inline static const juce::StringRef OptionsCommandCategoryName{"Options"};
        inline static const juce::StringRef ToggleModuleSelectorsShortName{"Toggle module selectors"};
        inline static const juce::StringRef ToggleModuleSelectorsDescription{"Toggles display of module selector lists"};

        inline static const juce::StringRef ModulesCommandCategoryName{"Modules"};
        static juce::String getRevealModuleSelectorShortName(const int moduleID) { return "Module " + juce::String{moduleID}; }
        static juce::String getRevealModuleSelectorDescription(const int moduleID) { return "Reveal module selector " + juce::String{moduleID}; }
    };

    class Identifiers
    {
    public:
        inline static const juce::Identifier StaticTreeType{"HoaParameters"};
    };

    class Sockets
    {
    public:
        static constexpr uint16_t DecoderMatrixCoefficientMessengerLocalPort{49165};
        static constexpr uint16_t DecoderMatrixCoefficientMessengerRemotePort{49165};
    };

    namespace UI
    {
        class Dimensions
        {
        public:
        };

        class Shortcuts
        {
        public:
            constexpr static int SwitchToHoaTabKeycode{'['};
            constexpr static int SwitchToNetworkTabKeycode{']'};
            constexpr static int RevealModuleSelectorListKeycode{'1'};
        };
    }
}

#endif //HOAUTILS_H
