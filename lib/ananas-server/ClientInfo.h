#ifndef CLIENTINFO_H
#define CLIENTINFO_H

#include <juce_events/juce_events.h>
#include <juce_data_structures/juce_data_structures.h>
#include "Packet.h"

namespace ananas
{
    template<typename AnnouncePacketType>
    class ClientInfo
    {
    public:
        void update(const AnnouncePacketType *packet);

        [[nodiscard]] bool isConnected() const;

        [[nodiscard]] AnnouncePacketType getInfo() const;

    private:
        AnnouncePacketType info{};
        uint32_t lastReceiveTime{0};
    };

    //==========================================================================

    template<typename AnnouncePacketType>
    class ModuleInfo
    {
    public:
        void update();

        [[nodiscard]] juce::ValueTree toValueTree() const;

        [[nodiscard]] bool isConnected() const;

        [[nodiscard]] bool justDisconnected();

        [[nodiscard]] bool justConnected();

        [[nodiscard]] std::pair<float, float> getSecondarySource0Position() const;

        [[nodiscard]] std::pair<float, float> getSecondarySource1Position() const;

        [[nodiscard]] juce::int32 getDecoderMatrixRow0() const;

        [[nodiscard]] juce::int32 getDecoderMatrixRow1() const;

        static ModuleInfo fromValueTree(const juce::ValueTree &tree);

    private:
        juce::uint32 lastReceiveTime{0};
        bool wasConnected{false};
        std::pair<float, float> secondarySource0Position;
        std::pair<float, float> secondarySource1Position;
        juce::int32 decoderMatrixRow0{0};
        juce::int32 decoderMatrixRow1{0};
    };

    //==========================================================================

    template<typename AnnouncePacketType>
    class ClientList final : public juce::Timer,
                             public juce::ChangeBroadcaster
    {
    public:
        ClientList();

        void handlePacket(const juce::String &clientIP, const AnnouncePacketType *packet);

        void timerCallback() override;

        [[nodiscard]] juce::var toVar() const;

        [[nodiscard]] bool getShouldReboot() const;

        void setShouldReboot(bool should);

        [[nodiscard]] uint getCount() const;

        juce::ValueTree toValueTree();

    private:
        void checkConnectivity();

        std::map<juce::String, ClientInfo<AnnouncePacketType> > clients;
        bool shouldReboot{false};
    };


    //==========================================================================

    template<typename AnnouncePacketType>
    class ModuleList final : public juce::ChangeBroadcaster,
                             public juce::Timer
    {
    public:
        ModuleList();

        void handlePacket(const juce::String &moduleIP);

        void timerCallback() override;

        [[nodiscard]] juce::var toVar() const;

        [[nodiscard]] juce::ValueTree toValueTree() const;

        void fromValueTree(const juce::ValueTree &tree);

    private:
        void checkConnectivity();

        std::map<juce::String, ModuleInfo<AnnouncePacketType> > modules;
    };
}


#endif //CLIENTINFO_H
