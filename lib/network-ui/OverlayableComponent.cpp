#include "OverlayableComponent.h"
#include <AnanasLookAndFeel.h>
#include "AnanasUIUtils.h"
#include <Server.h>

namespace ananas::UI
{
    OverlayableComponent::OverlayableComponent()
    {
        // Always create the overlay to begin with.
        overlay = std::make_unique<OverlayComponent>();
        addAndMakeVisible(overlay.get(), -1);
        overlay->setText(Strings::OverlayInitialText);
        overlay->toFront(true);
    }

    void OverlayableComponent::changeListenerCallback(juce::ChangeBroadcaster *source)
    {
        // if (!isVisible()) return;

        if (const auto *server = dynamic_cast<Server::Server *>(source)) {
            // If the server is connected and the overlay is present, get rid
            // of the overlay.
            if (const auto connected{server->isConnected()}; connected && overlay != nullptr) {
                overlay.reset();
            } else if (!connected) {
                // If not connected and there's no overlay, create the overlay
                // with text indicating lack of connection.
                if (overlay != nullptr) {
                    overlay = std::make_unique<OverlayComponent>();
                    addAndMakeVisible(overlay.get(), -1);
                    overlay->setText(Strings::OverlayNoNetworkText);
                }
                // Resize to trigger a redraw.
                resized();
            }
        }
    }

    void OverlayableComponent::resized()
    {
        if (overlay != nullptr) {
            overlay->setBounds(getLocalBounds());
            overlay->toFront(true);
        }
    }

    //==========================================================================

    void OverlayableComponent::OverlayComponent::paint(juce::Graphics &g)
    {
        if (auto *lnf{dynamic_cast<AnanasLookAndFeel *>(&getLookAndFeel())})
            lnf->drawOverlay(g, *this, text);
    }

    void OverlayableComponent::OverlayComponent::setText(const juce::String &textToDisplay)
    {
        text = textToDisplay;
    }
}
