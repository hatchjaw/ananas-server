#ifndef HOAINTERFACECOMPONENT_H
#define HOAINTERFACECOMPONENT_H

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

#include "ModuleComponent.h"
#include "SoundFieldComponent.h"
#include "SpeakerIconComponent.h"
#if SHOW_NO_NETWORK_OVERLAY
#include <OverlayableComponent.h>
#endif

namespace ananas::HOA::UI
{
    class HOAInterfaceComponent final :
#if SHOW_NO_NETWORK_OVERLAY
            public ananas::UI::OverlayableComponent,
#else
            public juce::Component,
#endif
            public juce::ValueTree::Listener,
            public juce::AudioProcessorValueTreeState::Listener
    {
    public:
        HOAInterfaceComponent(size_t numChannels,
                              juce::AudioProcessorValueTreeState &apvts,
                              juce::ValueTree &persistentTreeToListenTo);

        ~HOAInterfaceComponent() override;

        void paint(juce::Graphics &g) override;

        void resized() override;

        void updateModuleLists(const juce::var &var);

        void valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyHasChanged, const juce::Identifier &property) override;

        void parameterChanged(const juce::String &parameterID, float newValue) override;

        void expandModuleList(int moduleID);

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HOAInterfaceComponent)

        juce::AudioProcessorValueTreeState &state;
        SoundFieldComponent soundFieldComponent;
        juce::OwnedArray<ModuleComponent> modules;
        juce::OwnedArray<SpeakerIconComponent> speakerIcons;
        juce::ValueTree &persistentTree;
    };
}


#endif //HOAINTERFACECOMPONENT_H
