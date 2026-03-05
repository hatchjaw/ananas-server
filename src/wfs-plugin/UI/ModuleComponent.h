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

        void expandModuleList();

        void collapseModuleList();

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModuleComponent)

        void setSelectedModule(const juce::String &ip);

        juce::ValueTree &tree;
        bool showModuleSelector{false};
        juce::ComboBox moduleSelector;
        int index{0};
    };
} // ananas::WFS

#endif //MODULESELECTORCOMPONENT_H
