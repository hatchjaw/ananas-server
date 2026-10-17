#include "NetworkOverviewComponent.h"

#include <Packet.h>

namespace ananas::UI
{
    template <typename AnnouncePacketType>
    NetworkOverviewComponent<AnnouncePacketType>::NetworkOverviewComponent(juce::ValueTree &dynamicTree, juce::ValueTree &persistentTree)
        : switches(dynamicTree, persistentTree),
          timeAuthority(dynamicTree),
          clientOverview(dynamicTree)
    {
        addAndMakeVisible(switches);
        addAndMakeVisible(timeAuthority);
        addAndMakeVisible(clientOverview);
    }

    template <typename AnnouncePacketType>
    void NetworkOverviewComponent<AnnouncePacketType>::paint(juce::Graphics &g)
    {
        g.fillAll(findColour(backgroundColourId));
    }

    template <typename AnnouncePacketType>
    void NetworkOverviewComponent<AnnouncePacketType>::resized()
    {
        auto bounds{getLocalBounds()};
        switches.setBounds(bounds.removeFromTop(Dimensions::SwitchesSectionHeight));
        timeAuthority.setBounds(bounds.removeFromTop(Dimensions::TimeAuthoritySectionHeight));
        // Client overview gets remaining bounds.
        clientOverview.setBounds(bounds);

#if SHOW_NO_NETWORK_OVERLAY
        OverlayableComponent::resized();
#endif
    }

    template
    class NetworkOverviewComponent<ClientAnnouncePacket>;

    template
    class NetworkOverviewComponent<WfsClientAnnouncePacket>;

    template
    class NetworkOverviewComponent<HoaClientAnnouncePacket>;
}
