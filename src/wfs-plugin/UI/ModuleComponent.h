#ifndef MODULESELECTORCOMPONENT_H
#define MODULESELECTORCOMPONENT_H

#include <juce_gui_basics/juce_gui_basics.h>

namespace ananas::WFS::UI
{
    class ModuleComponent final : public juce::Component
    {
    public:
        explicit ModuleComponent(int moduleIndex, juce::ValueTree &persistentTree);

        void resized() override;

        void setAvailableModules(const juce::StringArray &ips);

        void setIndexForModule() const;

        void shouldShowModuleSelector(bool show);

        class SpeakerIconComponent final : public Component
        {
        public:
            //==================================================================

            enum ColourIds
            {
                fillColourId = 0x2001700,
                borderColourId = 0x2001701
            };

            //==================================================================

            void paint(juce::Graphics &g) override;
        };

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModuleComponent)

        void setSelectedModule(const juce::var &var);

        juce::ValueTree &tree;
        bool showModuleSelector{false};
        juce::ComboBox moduleSelector;
        int index{0};
        SpeakerIconComponent speakerIcon1, speakerIcon2;
    };
} // ananas::WFS

#endif //MODULESELECTORCOMPONENT_H
