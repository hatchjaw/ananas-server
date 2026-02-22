#ifndef WFSMESSENGER_H
#define WFSMESSENGER_H

#include <AnanasUtils.h>
#include <Server.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_osc/juce_osc.h>

#include "WFSUtils.h"

namespace ananas::WFS
{
    class WFSMessenger final : public juce::Thread,
                               public juce::OSCSender,
                               public juce::AudioProcessorValueTreeState::Listener,
                               public juce::ValueTree::Listener,
                               public juce::Timer
    {
    public:
        explicit WFSMessenger(const ananas::Utils::SenderThreadSocketParams &p);

        bool connect();

        void run() override;

        void valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyHasChanged, const ::juce::Identifier &property) override;

        void parameterChanged(const juce::String &parameterID, float newValue) override;

        void timerCallback() override;

    private:
        struct ParamSlot
        {
            std::atomic<float> value{0.0f};
            std::atomic<bool> changed{false};
        };

        void runImpl() const;

        juce::DatagramSocket socket;
        juce::String ip;
        juce::uint16 localPort, remotePort;
        bool connected{false};
        std::unordered_map<juce::String, ParamSlot> slots;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WFSMessenger);
    };
} // ananas::WFS

#endif //WFSMESSENGER_H
