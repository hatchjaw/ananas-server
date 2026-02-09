#include "AnanasLookAndFeel.h"

#include <NetworkOverviewComponent.h>

namespace ananas::UI
{
    AnanasLookAndFeel::AnanasLookAndFeel()
    {
        // Background
        setColour(juce::ResizableWindow::backgroundColourId, juce::Colours::ghostwhite);

        // Text buttons
        setColour(juce::TextButton::buttonColourId, juce::Colours::lightseagreen);
        setColour(juce::TextButton::buttonOnColourId, juce::Colours::lightseagreen.darker());
        setColour(juce::TextButton::textColourOffId, juce::Colours::black.withAlpha(0.5f));
        setColour(juce::TextButton::textColourOnId, juce::Colours::black);

        // No-network overlay
        setColour(OverlayableComponent::OverlayComponent::backgroundColourId, juce::Colours::black.withAlpha(0.25f));
        setColour(OverlayableComponent::OverlayComponent::boxShadowColourId, juce::Colours::black.withAlpha(0.5f));
        setColour(OverlayableComponent::OverlayComponent::boxBackgroundColourId, juce::Colours::whitesmoke);
        setColour(OverlayableComponent::OverlayComponent::boxBorderColourId, juce::Colours::slategrey);
        setColour(OverlayableComponent::OverlayComponent::boxTextColourId, juce::Colours::black);

        // Network overview
        setColour(NetworkOverviewComponent::backgroundColourId, juce::Colours::transparentWhite);

        // Network overview tables
        setColour(juce::TableListBox::backgroundColourId, juce::Colours::transparentWhite);
        setColour(juce::TableListBox::outlineColourId, juce::Colours::black);
        setColour(juce::TableListBox::textColourId, juce::Colours::red); //?
        setColour(AnanasNetworkTable::textColourId, juce::Colours::black);
        setColour(AnanasNetworkTable::okColourId, juce::Colours::lightseagreen);
        setColour(AnanasNetworkTable::warningColourId, juce::Colours::palevioletred);

        // Switches
        setColour(SwitchesComponent::backgroundColourId, juce::Colours::transparentWhite);

        // Time authority
        setColour(TimeAuthorityComponent::backgroundColourId, juce::Colours::transparentWhite);

        // Clients
        setColour(ClientsOverviewComponent::backgroundColourId, juce::Colours::transparentWhite);
    }

    int AnanasLookAndFeel::getTabButtonBestWidth(juce::TabBarButton &tabBarButton, const int tabDepth)
    {
        return totalNumTabs > 0 ? totalWidth / totalNumTabs : LookAndFeel_V4::getTabButtonBestWidth(tabBarButton, tabDepth);
    }

    void AnanasLookAndFeel::setTotalWidth(const int width)
    {
        totalWidth = width;
    }

    void AnanasLookAndFeel::setNumberOfTabs(const int numTabs)
    {
        totalNumTabs = numTabs;
    }

    void AnanasLookAndFeel::drawOverlay(juce::Graphics &g, const OverlayableComponent::OverlayComponent &o, const juce::String &text)
    {
        const auto bounds{o.getLocalBounds()};

        // Semi-transparent background
        g.fillAll(o.findColour(OverlayableComponent::OverlayComponent::backgroundColourId));

        // Calculate centered box
        const auto box{
            juce::Rectangle(getOverlayBoxWidth(), getOverlayBoxHeight())
            .withCentre(bounds.getCentre())
        };

        // Create shadow
        const juce::DropShadow shadow{
            o.findColour(OverlayableComponent::OverlayComponent::boxShadowColourId),
            static_cast<int>(getOverlayBoxBorderRadius() * 2),
            juce::Point{
                getOverlayBoxShadowOffset(),
                getOverlayBoxShadowOffset()
            }
        };

        // Draw the shadow
        shadow.drawForRectangle(g, box);

        // Draw opaque box
        g.setColour(o.findColour(OverlayableComponent::OverlayComponent::boxBackgroundColourId));
        g.fillRoundedRectangle(box.toFloat(), getOverlayBoxBorderRadius());

        // Add border
        g.setColour(o.findColour(OverlayableComponent::OverlayComponent::boxBorderColourId));
        g.drawRoundedRectangle(box.toFloat(), getOverlayBoxBorderRadius(), getOverlayBoxBorderThickness());

        // Draw text
        g.setColour(o.findColour(OverlayableComponent::OverlayComponent::boxTextColourId));
        g.setFont(getOverlayBoxTextSize());
        g.drawText(text, box, juce::Justification::centred);
    }

    void AnanasLookAndFeel::drawTableHeaderColumn(
        juce::Graphics &g,
        juce::TableHeaderComponent &header,
        const juce::String &columnName,
        const int columnId,
        const int width,
        const int height,
        const bool isMouseOver,
        const bool isMouseDown,
        const int columnFlags
    )
    {
        const auto highlightColour{header.findColour(juce::TableHeaderComponent::highlightColourId)};

        if (isMouseDown)
            g.fillAll(highlightColour);
        else if (isMouseOver)
            g.fillAll(highlightColour.withMultipliedAlpha(0.625f));

        juce::Rectangle area(width, height);
        area.reduce(4, 0);

        if ((columnFlags & (juce::TableHeaderComponent::sortedForwards | juce::TableHeaderComponent::sortedBackwards)) != 0) {
            juce::Path sortArrow;
            sortArrow.addTriangle(0.0f, 0.0f,
                                  0.5f, (columnFlags & juce::TableHeaderComponent::sortedForwards) != 0 ? -0.8f : 0.8f,
                                  1.0f, 0.0f);

            g.setColour(juce::Colour(0x99000000));
            g.fillPath(sortArrow, sortArrow.getTransformToScaleToFit(area.removeFromRight(height / 2).reduced(2).toFloat(), true));
        }

        g.setColour(header.findColour(juce::TableHeaderComponent::textColourId));
        g.setFont(withDefaultMetrics(juce::FontOptions(static_cast<float>(height) * 0.5f, juce::Font::bold)));
        g.drawFittedText(columnName, area, getTableHeaderJustification(columnId), 1);
    }
}
