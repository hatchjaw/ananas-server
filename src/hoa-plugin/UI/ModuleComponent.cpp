#include "ModuleComponent.h"

#include <AnanasUtils.h>

namespace ananas::HOA::UI
{
    ModuleComponent::ModuleComponent(const size_t idx, juce::ValueTree &persistentTree)
        : tree(persistentTree), index(idx)
    {
        decoderMatrixRows.first = index * 2;
        decoderMatrixRows.second = index * 2 + 1;

        addAndMakeVisible(moduleSelector);

        moduleSelector.onChange = [this]
        {
            setDecoderMaxtrixRowsForModule();
        };
    }

    void ModuleComponent::setDecoderMaxtrixRowsForModule() const
    {
        const auto selected{moduleSelector.getText()};

        const auto modules{tree.getProperty(Utils::Identifiers::ModulesParamID)};
        if (auto *obj = modules.getDynamicObject()) {
            for (const auto &prop: obj->getProperties()) {
                const auto module{obj->getProperty(prop.name).getDynamicObject()};

                if (prop.name.toString() == selected) {
                    // If the module's IP matches this selector's text, set the module's decoder
                    // matrix rows equal to those associated with this selector.
                    module->setProperty(Utils::Identifiers::ModuleDecoderMatrixRow1PropertyID, decoderMatrixRows.first);
                    module->setProperty(Utils::Identifiers::ModuleDecoderMatrixRow2PropertyID, decoderMatrixRows.second);
                    tree.sendPropertyChangeMessage(Utils::Identifiers::ModulesParamID);
                }
            }
        }
    }

    void ModuleComponent::resized()
    {
        moduleSelector.setBounds(getLocalBounds());
    }

    void ModuleComponent::collapseModuleList()
    {
        moduleSelector.hidePopup();
    }

    void ModuleComponent::expandModuleList()
    {
        moduleSelector.showPopup();
    }

    void ModuleComponent::setAvailableModules(const juce::StringArray &ips)
    {
        moduleSelector.clear(juce::dontSendNotification);
        moduleSelector.addItemList(ips, 1);
    }

    juce::Rectangle<int> ModuleComponent::getTheBounds() const
    {
        switch (index) {
            case 0:
                return {700, 80, 100, 25};
            case 1:
                return {900, 560, 100, 25};
            case 2:
                return {400, 765, 100, 25};
            case 3:
                return {190, 280, 100, 25};
            case 4:
                return {550, 185, 100, 25};
            case 5:
                return {800, 425, 100, 25};
            case 6:
                return {550, 660, 100, 25};
            case 7:
                return {300, 425, 100, 25};
            default:
                return {550, 425, 100, 25};
        }
    }
}
