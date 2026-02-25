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
        const auto varIntN{juce::var{index}};
        const auto defaultModuleID{juce::var{0}};
        if (auto *obj = modules.getDynamicObject()) {
            for (const auto &prop: obj->getProperties()) {
                if (prop.value.getProperty(ananas::Utils::Identifiers::ModuleIDPropertyID, defaultModuleID) == varIntN) {
                    setSelectedModule(prop.name.toString());
                }
            }
        }
    }

    void ModuleComponent::setIndexForModule() const
    {
        const auto modules{tree.getProperty(ananas::Utils::Identifiers::ModulesParamID)};
        if (auto *obj = modules.getDynamicObject()) {
            for (const auto &prop: obj->getProperties()) {
                if (prop.name.toString() == moduleSelector.getText()) {
                    const auto module{obj->getProperty(prop.name).getDynamicObject()};
                    module->setProperty(ananas::Utils::Identifiers::ModuleIDPropertyID, static_cast<int>(index));
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
