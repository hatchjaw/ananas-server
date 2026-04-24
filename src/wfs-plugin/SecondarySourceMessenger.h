#ifndef SECONDARYSOURCEMESSENGER_H
#define SECONDARYSOURCEMESSENGER_H

#include <juce_data_structures/juce_data_structures.h>
#include <juce_osc/juce_osc.h>

namespace ananas::WFS
{
    class SecondarySourceMessenger final : public juce::ValueTree::Listener,
                                           public juce::OSCSender
    {
    public:
        SecondarySourceMessenger();

        void valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyHasChanged, const juce::Identifier &property) override;

    private:
        juce::DatagramSocket socket;
    };
}

#endif //SECONDARYSOURCEMESSENGER_H
