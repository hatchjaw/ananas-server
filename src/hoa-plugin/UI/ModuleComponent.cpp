#include "ModuleComponent.h"

#include <AnanasUtils.h>

namespace ananas::HOA::UI
{
    ModuleComponent::ModuleComponent(const size_t idx, juce::ValueTree &persistentTree)
        : tree(persistentTree), index(idx)
    {
        decoderMatrixRows.first = (index - 1) * 2;
        decoderMatrixRows.second = (index - 1) * 2 + 1;

        addAndMakeVisible(moduleSelector);

        moduleSelector.onChange = [this]
        {
            setDecoderMatrixRowsForModule();
        };
    }

    void ModuleComponent::setDecoderMatrixRowsForModule() const
    {
        const auto selected{moduleSelector.getText()};

        const auto modules{tree.getProperty(Utils::Identifiers::ModulesParamID)};
        if (auto *obj = modules.getDynamicObject()) {
            for (const auto &prop: obj->getProperties()) {
                const auto module{obj->getProperty(prop.name).getDynamicObject()};

                if (prop.name.toString() == selected) {
                    // If the module's IP matches this selector's text, set the module's decoder
                    // matrix rows equal to those associated with this selector.
                    module->setProperty(Utils::Identifiers::ModuleDecoderMatrixRow0PropertyID, static_cast<int>(decoderMatrixRows.first));
                    module->setProperty(Utils::Identifiers::ModuleDecoderMatrixRow1PropertyID, static_cast<int>(decoderMatrixRows.second));
                    module->setProperty(Utils::Identifiers::ModulePositionHasChangedPropertyID, true);
                    tree.sendPropertyChangeMessage(Utils::Identifiers::ModulesParamID);
                }
            }
        }
    }

    void ModuleComponent::setSelectedModule(const juce::String &ip)
    {
        moduleSelector.setText(ip, juce::dontSendNotification);
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

        const auto modules{tree.getProperty(ananas::Utils::Identifiers::ModulesParamID)};

        if (auto *obj = modules.getDynamicObject()) {
            for (const auto &prop: obj->getProperties()) {
                const auto module{obj->getProperty(prop.name).getDynamicObject()};

                if (module->getProperty(Utils::Identifiers::ModuleDecoderMatrixRow0PropertyID) ==
                    juce::var{static_cast<int>(decoderMatrixRows.first)}
                    &&
                    module->getProperty(Utils::Identifiers::ModuleDecoderMatrixRow1PropertyID) ==
                    juce::var{static_cast<int>(decoderMatrixRows.second)}) {
                    setSelectedModule(prop.name.toString());
                }
            }
        }
    }

    juce::Rectangle<int> ModuleComponent::getTheBounds() const
    {
        switch (index) {
            case 1:
                return {675, 80, 150, 25};
            case 2:
                return {875, 560, 150, 25};
            case 3:
                return {375, 765, 150, 25};
            case 4:
                return {165, 280, 150, 25};
            case 5:
                return {525, 165, 150, 25};
            case 6:
                return {775, 425, 150, 25};
            case 7:
                return {525, 680, 150, 25};
            case 8:
                return {275, 425, 150, 25};
            default:
                return {525, 425, 150, 25};
        }
    }
}
