#ifndef MODULECOMPONENT_H
#define MODULECOMPONENT_H
#include <juce_gui_basics/juce_gui_basics.h>


namespace ananas::HOA::UI
{
    class ModuleComponent : public juce::Component
    {
    public:
        ModuleComponent(size_t idx, juce::ValueTree &persistentTree);

        void resized() override;

        void collapseModuleList();

        void expandModuleList();

        void setAvailableModules(const juce::StringArray &ips);

        [[nodiscard]] juce::Rectangle<int> getTheBounds() const;

        void setDecoderMaxtrixRowsForModule() const;

    private:
        juce::ValueTree &tree;
        juce::ComboBox moduleSelector;
        size_t index{0};
        std::pair<int, int> decoderMatrixRows;
    };
}


#endif //MODULECOMPONENT_H
