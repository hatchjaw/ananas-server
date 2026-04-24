#ifndef MODULESELECTORCOMPONENT_H
#define MODULESELECTORCOMPONENT_H

#include <juce_gui_basics/juce_gui_basics.h>

namespace ananas::WFS::UI
{
    class ModuleComponent final : public juce::Component
    {
    public:
        ModuleComponent(float ss0x, float ss0y, float ss1x, float ss1y, juce::ValueTree &persistentTree);

        void resized() override;

        void setAvailableModules(const juce::StringArray &ips);

        void setCoordinatesForModule() const;

        void shouldShowModuleSelector(bool show);

        void expandModuleList();

        void collapseModuleList();

        void setSecondarySourceCoordinates(float ss0x, float ss0y, float ss1x, float ss1y);

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModuleComponent)

        void setSelectedModule(const juce::String &ip);

        juce::ValueTree &tree;
        bool showModuleSelector{false};
        juce::ComboBox moduleSelector;
        std::pair<float, float> secondarySource0Position;
        std::pair<float, float> secondarySource1Position;
    };
} // ananas::WFS

#endif //MODULESELECTORCOMPONENT_H
