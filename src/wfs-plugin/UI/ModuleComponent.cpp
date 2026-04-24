#include "ModuleComponent.h"
#include <AnanasUtils.h>
#include "WfsLookAndFeel.h"
#include "../WFSUtils.h"

namespace ananas::WFS::UI
{
    ModuleComponent::ModuleComponent(
        const float ss0x,
        const float ss0y,
        const float ss1x,
        const float ss1y,
        juce::ValueTree &persistentTree
    )
        : tree(persistentTree),
          secondarySource0Position(std::make_pair(ss0x, ss0y)),
          secondarySource1Position(std::make_pair(ss1x, ss1y))
    {
        addAndMakeVisible(moduleSelector);

        moduleSelector.onChange = [this]
        {
            setCoordinatesForModule();
        };
    }

    void ModuleComponent::resized()
    {
        if (showModuleSelector) {
            const auto comboBoxBounds{getLocalBounds().removeFromTop(Dimensions::ModuleSelectorHeight)};
            moduleSelector.setBounds(comboBoxBounds);
            moduleSelector.setVisible(true);
        } else {
            moduleSelector.setVisible(false);
            setBounds(0, 0, 0, 0);
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

                if (module->getProperty(ananas::Utils::Identifiers::ModuleSecondarySource0xPropertyID) == juce::var{secondarySource0Position.first} &&
                    module->getProperty(ananas::Utils::Identifiers::ModuleSecondarySource0yPropertyID) == juce::var{secondarySource0Position.second} &&
                    module->getProperty(ananas::Utils::Identifiers::ModuleSecondarySource1xPropertyID) == juce::var{secondarySource1Position.first} &&
                    module->getProperty(ananas::Utils::Identifiers::ModuleSecondarySource1yPropertyID) == juce::var{secondarySource1Position.second}) {
                    setSelectedModule(prop.name.toString());
                }
            }
        }
    }

    void ModuleComponent::setCoordinatesForModule() const
    {
        const auto selected{moduleSelector.getText()};

        const auto modules{tree.getProperty(ananas::Utils::Identifiers::ModulesParamID)};
        if (auto *obj = modules.getDynamicObject()) {
            for (const auto &prop: obj->getProperties()) {
                const auto module{obj->getProperty(prop.name).getDynamicObject()};

                if (prop.name.toString() == selected) {
                    // If the module's IP matches this selector's text, set the module's secondary
                    // source positions equal to the positions associated with the selector.
                    module->setProperty(ananas::Utils::Identifiers::ModuleSecondarySource0xPropertyID, secondarySource0Position.first);
                    module->setProperty(ananas::Utils::Identifiers::ModuleSecondarySource0yPropertyID, secondarySource0Position.second);
                    module->setProperty(ananas::Utils::Identifiers::ModuleSecondarySource1xPropertyID, secondarySource1Position.first);
                    module->setProperty(ananas::Utils::Identifiers::ModuleSecondarySource1yPropertyID, secondarySource1Position.second);
                    module->setProperty(ananas::Utils::Identifiers::ModulePositionHasChangedPropertyID, true);
                    tree.sendPropertyChangeMessage(ananas::Utils::Identifiers::ModulesParamID);
                } else if (module->getProperty(ananas::Utils::Identifiers::ModuleSecondarySource0xPropertyID) == juce::var{secondarySource0Position.first} &&
                           module->getProperty(ananas::Utils::Identifiers::ModuleSecondarySource0yPropertyID) == juce::var{secondarySource0Position.second} &&
                           module->getProperty(ananas::Utils::Identifiers::ModuleSecondarySource1xPropertyID) == juce::var{secondarySource1Position.first} &&
                           module->getProperty(ananas::Utils::Identifiers::ModuleSecondarySource1yPropertyID) == juce::var{secondarySource1Position.second}) {
                    // If the module's ID matches the index of this selector, it's being replaced,
                    // so set its position to the origin...
                    module->setProperty(ananas::Utils::Identifiers::ModuleSecondarySource0xPropertyID, 0.f);
                    module->setProperty(ananas::Utils::Identifiers::ModuleSecondarySource0yPropertyID, 0.f);
                    module->setProperty(ananas::Utils::Identifiers::ModuleSecondarySource1xPropertyID, 0.f);
                    module->setProperty(ananas::Utils::Identifiers::ModuleSecondarySource1yPropertyID, 0.f);
                    module->setProperty(ananas::Utils::Identifiers::ModulePositionHasChangedPropertyID, true);
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

    void ModuleComponent::setSecondarySourceCoordinates(const float ss0x, const float ss0y, const float ss1x, const float ss1y)
    {
        secondarySource0Position.first = ss0x;
        secondarySource0Position.second = ss0y;
        secondarySource1Position.first = ss1x;
        secondarySource1Position.second = ss1y;
    }

    void ModuleComponent::setSelectedModule(const juce::String &ip)
    {
        moduleSelector.setText(ip, juce::dontSendNotification);
    }
} // ananas::WFS
