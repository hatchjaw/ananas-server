#include "HOAInterfaceComponent.h"

#include <AnanasUIUtils.h>
#include <AnanasUtils.h>

namespace ananas::HOA::UI
{
    HOAInterfaceComponent::HOAInterfaceComponent(
        const size_t numChannels,
        juce::AudioProcessorValueTreeState &apvts,
        juce::ValueTree &persistentTreeToListenTo)
        : state(apvts),
          persistentTree(persistentTreeToListenTo)
    {
        addAndMakeVisible(soundFieldComponent);

        for (size_t n{1}; n <= numChannels; ++n) {
            const auto s{speakerIcons.add(new SpeakerIconComponent(n, apvts))};
            addAndMakeVisible(s);
        }

        for (size_t n{1}; n <= numChannels / 2; ++n) {
            const auto m{modules.add(new ModuleComponent(n, persistentTree))};
            addAndMakeVisible(m);
        }

        // Listen to the persistent tree for module selection changes.
        persistentTree.addListener(this);
        // Trigger an initial property change so that combo boxes get populated.
        persistentTree.sendPropertyChangeMessage(Utils::Identifiers::ModulesParamID);
    }

    HOAInterfaceComponent::~HOAInterfaceComponent()
    {
        persistentTree.removeListener(this);
    }

    void HOAInterfaceComponent::paint(juce::Graphics &g)
    {
        g.fillAll(juce::Colours::transparentWhite);
    }

    void HOAInterfaceComponent::resized()
    {
        auto bounds{getLocalBounds()};
        bounds = bounds.reduced(10);
        soundFieldComponent.setBounds(bounds);

        for (const auto &s: speakerIcons) {
            s->setBounds(s->getTheBounds());
        }

        for (const auto &m: modules) {
            m->setBounds(m->getTheBounds());
        }

#if SHOW_NO_NETWORK_OVERLAY
        OverlayableComponent::resized();
#endif
    }

    void HOAInterfaceComponent::updateModuleLists(const juce::var &var)
    {
        juce::StringArray ips;
        if (auto *obj = var.getDynamicObject()) {
            for (const auto &prop: obj->getProperties()) {
                auto module{obj->getProperty(prop.name).getDynamicObject()};
                if (module->getProperty(ananas::Utils::Identifiers::ModuleIsConnectedPropertyID)) {
                    ips.add(prop.name.toString());
                }
            }
        }
        for (const auto &m: modules) {
            m->setAvailableModules(ips);
        }
    }

    void HOAInterfaceComponent::valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyHasChanged, const juce::Identifier &property)
    {
        if (property == Utils::Identifiers::ModulesParamID) {
            updateModuleLists(treeWhosePropertyHasChanged[property]);
        }
    }

    void HOAInterfaceComponent::parameterChanged(const juce::String &parameterID, float newValue)
    {
    }

    void HOAInterfaceComponent::expandModuleList(int moduleID)
    {
        for (auto *m: modules) {
            m->collapseModuleList();
        }
        modules[moduleID]->expandModuleList();
    }
}
