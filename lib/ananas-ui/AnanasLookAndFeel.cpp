#include "AnanasLookAndFeel.h"
#include <AnanasFonts.h>
#include <NetworkOverviewComponent.h>

namespace ananas::UI
{
    AnanasLookAndFeel::AnanasLookAndFeel()
        : typefaceSans{
              juce::Typeface::createSystemTypefaceFor(BinaryData::FF_Unit_Pro_Regular_otf,
                                                      BinaryData::FF_Unit_Pro_Regular_otfSize)
          },
          typefaceBold{
              juce::Typeface::createSystemTypefaceFor(BinaryData::FF_Unit_Pro_Bold_otf,
                                                      BinaryData::FF_Unit_Pro_Bold_otfSize)
          }
    {
        darkTextColour = juce::Colours::black.brighter(.3);
        lightTextColour = juce::Colours::white.darker(.05);
        okColour = juce::Colours::lightseagreen;
        warningColour = juce::Colours::palevioletred;

        // Background
        setColour(juce::ResizableWindow::backgroundColourId, juce::Colours::ghostwhite);

        // Labels
        setColour(juce::Label::textColourId, darkTextColour);

        // Text buttons
        setColour(juce::TextButton::buttonColourId, okColour);
        setColour(juce::TextButton::buttonOnColourId, okColour.darker());
        setColour(juce::TextButton::textColourOffId, lightTextColour);
        setColour(juce::TextButton::textColourOnId, darkTextColour);

        // Toggle buttons
        setColour(juce::ToggleButton::textColourId, darkTextColour);
        setColour(juce::ToggleButton::tickColourId, darkTextColour);
        setColour(juce::ToggleButton::tickDisabledColourId, darkTextColour);

        // Sliders
        setColour(juce::Slider::textBoxTextColourId, darkTextColour);

        // Comboboxes
        setColour(juce::ComboBox::backgroundColourId, lightTextColour);
        setColour(juce::ComboBox::textColourId, darkTextColour);
        setColour(juce::ComboBox::outlineColourId, darkTextColour);
        setColour(juce::ComboBox::arrowColourId, okColour);
        setColour(juce::ComboBox::focusedOutlineColourId, darkTextColour.brighter(.25));

        // Popup menus
        setColour(juce::PopupMenu::backgroundColourId, lightTextColour);
        setColour(juce::PopupMenu::textColourId, darkTextColour);
        setColour(juce::PopupMenu::highlightedBackgroundColourId, juce::Colour(0xff0066cc));
        setColour(juce::PopupMenu::highlightedTextColourId, lightTextColour);
        setColour(juce::PopupMenu::headerTextColourId, darkTextColour.withAlpha(0.6f));

        // Tooltips
        setColour(juce::TooltipWindow::backgroundColourId, darkTextColour);
        setColour(juce::TooltipWindow::textColourId, lightTextColour);
        setColour(juce::TooltipWindow::outlineColourId, lightTextColour);

        // Text fields
        setColour(juce::TextEditor::backgroundColourId, lightTextColour.darker(.1));
        setColour(juce::TextEditor::textColourId, darkTextColour);
        setColour(juce::TextEditor::outlineColourId, lightTextColour);

        // No-network overlay
        setColour(OverlayableComponent::OverlayComponent::backgroundColourId, darkTextColour.withAlpha(0.25f));
        setColour(OverlayableComponent::OverlayComponent::boxShadowColourId, darkTextColour.withAlpha(0.5f));
        setColour(OverlayableComponent::OverlayComponent::boxBackgroundColourId, juce::Colours::whitesmoke);
        setColour(OverlayableComponent::OverlayComponent::boxBorderColourId, juce::Colours::slategrey);
        setColour(OverlayableComponent::OverlayComponent::boxTextColourId, darkTextColour);

        // Network overview
        setColour(NetworkOverviewComponent::backgroundColourId, juce::Colours::transparentWhite);

        // Network overview tables
        setColour(juce::TableListBox::backgroundColourId, juce::Colours::transparentWhite);
        setColour(juce::TableListBox::outlineColourId, darkTextColour);
        setColour(juce::TableHeaderComponent::textColourId, darkTextColour);
        setColour(juce::TableHeaderComponent::outlineColourId, darkTextColour);
        setColour(AnanasNetworkTable::textColourId, darkTextColour);
        setColour(AnanasNetworkTable::okColourId, okColour);
        setColour(AnanasNetworkTable::warningColourId, warningColour);

        // Switches
        setColour(SwitchesComponent::backgroundColourId, juce::Colours::transparentWhite);

        // Time authority
        setColour(TimeAuthorityComponent::backgroundColourId, juce::Colours::transparentWhite);

        // Clients
        setColour(ClientsOverviewComponent::backgroundColourId, juce::Colours::transparentWhite);
    }

    juce::Typeface::Ptr AnanasLookAndFeel::getTypefaceForFont(const juce::Font &font)
    {
        if (typefaceSans == nullptr)
            return LookAndFeel_V4::getTypefaceForFont(font);

        const auto style{font.getTypefaceStyle()};

        if (style.containsIgnoreCase("bold")) {
            if (typefaceBold != nullptr)
                return typefaceBold;
        }

        return typefaceSans;
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

    void AnanasLookAndFeel::drawOverlay(juce::Graphics &g, const OverlayableComponent::OverlayComponent &o, const juce::String &text) const
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
        g.setFont(withDefaultMetrics(juce::FontOptions{static_cast<float>(height) * 0.5f, juce::Font::bold}));
        g.drawFittedText(columnName, area, getTableHeaderJustification(columnId), 1);
    }

    juce::Font AnanasLookAndFeel::getTextButtonFont(juce::TextButton &text_button, int buttonHeight)
    {
        return withDefaultMetrics(juce::FontOptions{juce::jmin(16.0f, static_cast<float>(buttonHeight) * 0.6f), juce::Font::bold});
    }
}
