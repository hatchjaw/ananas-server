#include "ModuleComponent.h"
#include <AnanasUtils.h>
#include "WfsLookAndFeel.h"
#include "../WFSUtils.h"

namespace ananas::WFS::UI
{
    ModuleComponent::ModuleComponent(const int moduleIndex, juce::ValueTree &persistentTree)
        : tree(persistentTree),
          index(moduleIndex)
    {
        addAndMakeVisible(moduleSelector);

        moduleSelector.onChange = [this]
        {
            setIndexForModule();
        };
    }

    void ModuleComponent::resized()
    {
        auto bounds{getLocalBounds()};

        if (showModuleSelector) {
            const auto comboBoxBounds{bounds.removeFromTop(Dimensions::ModuleSelectorHeight)};
            moduleSelector.setBounds(comboBoxBounds);
        } else {
            moduleSelector.setBounds(0, 0, 0, 0);
        }
    }

    void ModuleComponent::setAvailableModules(const juce::StringArray &ips)
    {
        moduleSelector.clear(juce::dontSendNotification);
        moduleSelector.addItemList(ips, 1);

        const auto modules{tree.getProperty(ananas::Utils::Identifiers::ModulesParamID)};

        if (auto *obj = modules.getDynamicObject()) {
            for (const auto &prop: obj->getProperties()) {
                const auto module{obj->getProperty(prop.name).getDynamicObject()};

                if (module->getProperty(ananas::Utils::Identifiers::ModuleIDPropertyID) == juce::var{index}) {
                    setSelectedModule(prop.name.toString());
                }
            }
        }
    }

    void ModuleComponent::setIndexForModule() const
    {
        const auto selected{moduleSelector.getText()};

        const auto modules{tree.getProperty(ananas::Utils::Identifiers::ModulesParamID)};
        if (auto *obj = modules.getDynamicObject()) {
            for (const auto &prop: obj->getProperties()) {
                const auto module{obj->getProperty(prop.name).getDynamicObject()};

                if (prop.name.toString() == selected) {
                    // If the module's IP matches this selector's text, set the module's index equal
                    // to the index of this selector.
                    module->setProperty(ananas::Utils::Identifiers::ModuleIDPropertyID, index);
                    module->setProperty(ananas::Utils::Identifiers::ModuleIDHasChangedPropertyID, true);
                    tree.sendPropertyChangeMessage(ananas::Utils::Identifiers::ModulesParamID);
                } else if (module->getProperty(ananas::Utils::Identifiers::ModuleIDPropertyID) == juce::var{index}) {
                    // If the module's ID matches the index of this selector, it's being replaced,
                    // so set its ID to -1.
                    module->setProperty(ananas::Utils::Identifiers::ModuleIDPropertyID, -1);
                    module->setProperty(ananas::Utils::Identifiers::ModuleIDHasChangedPropertyID, true);
                    tree.sendPropertyChangeMessage(ananas::Utils::Identifiers::ModulesParamID);
                }
            }
        }
    }

    void ModuleComponent::shouldShowModuleSelector(const bool show)
    {
        showModuleSelector = show;
        resized();
    }

    void ModuleComponent::expandModuleList()
    {
        moduleSelector.showPopup();
    }

    void ModuleComponent::collapseModuleList()
    {
        moduleSelector.hidePopup();
    }

    void ModuleComponent::setSelectedModule(const juce::var &var)
    {
        if (var.isString()) {
            moduleSelector.setText(var.toString(), juce::dontSendNotification);
        }
    }
} // ananas::WFS
