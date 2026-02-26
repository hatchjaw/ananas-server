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
        darkColour = juce::Colours::black.brighter(.3);
        lightColour = juce::Colours::white.darker(.05);
        okColour = juce::Colours::lightseagreen;
        warningColour = juce::Colours::palevioletred;

        // Background
        setColour(juce::ResizableWindow::backgroundColourId, juce::Colours::ghostwhite);

        // Labels
        setColour(juce::Label::textColourId, darkColour);

        // Text buttons
        setColour(juce::TextButton::buttonColourId, okColour);
        setColour(juce::TextButton::buttonOnColourId, okColour.darker());
        setColour(juce::TextButton::textColourOffId, lightColour);
        setColour(juce::TextButton::textColourOnId, darkColour);

        // Toggle buttons
        setColour(juce::ToggleButton::textColourId, darkColour);
        setColour(juce::ToggleButton::tickColourId, darkColour);
        setColour(juce::ToggleButton::tickDisabledColourId, darkColour);

        // Sliders
        setColour(juce::Slider::textBoxTextColourId, darkColour);

        // Combo boxes
        setColour(juce::ComboBox::backgroundColourId, lightColour);
        setColour(juce::ComboBox::textColourId, darkColour);
        setColour(juce::ComboBox::outlineColourId, darkColour);
        setColour(juce::ComboBox::arrowColourId, okColour);
        setColour(juce::ComboBox::focusedOutlineColourId, darkColour.brighter(.25));

        // Popup menus
        setColour(juce::PopupMenu::backgroundColourId, lightColour);
        setColour(juce::PopupMenu::textColourId, darkColour);
        setColour(juce::PopupMenu::highlightedBackgroundColourId, juce::Colour(0xff0066cc));
        setColour(juce::PopupMenu::highlightedTextColourId, lightColour);
        setColour(juce::PopupMenu::headerTextColourId, darkColour.withAlpha(0.6f));

        // Tooltips
        setColour(juce::TooltipWindow::backgroundColourId, darkColour);
        setColour(juce::TooltipWindow::textColourId, lightColour);
        setColour(juce::TooltipWindow::outlineColourId, darkColour);

        // Text fields
        setColour(juce::TextEditor::backgroundColourId, lightColour.darker(.1));
        setColour(juce::TextEditor::textColourId, darkColour);
        setColour(juce::TextEditor::outlineColourId, lightColour);

        // No-network overlay
        setColour(OverlayableComponent::OverlayComponent::backgroundColourId, darkColour.withAlpha(0.25f));
        setColour(OverlayableComponent::OverlayComponent::boxShadowColourId, darkColour.withAlpha(0.5f));
        setColour(OverlayableComponent::OverlayComponent::boxBackgroundColourId, juce::Colours::whitesmoke);
        setColour(OverlayableComponent::OverlayComponent::boxBorderColourId, juce::Colours::slategrey);
        setColour(OverlayableComponent::OverlayComponent::boxTextColourId, darkColour);

        // Network overview
        setColour(NetworkOverviewComponent::backgroundColourId, juce::Colours::transparentWhite);

        // Network overview tables
        setColour(juce::TableListBox::backgroundColourId, juce::Colours::transparentWhite);
        setColour(juce::TableListBox::outlineColourId, darkColour);
        setColour(juce::TableHeaderComponent::textColourId, darkColour);
        setColour(juce::TableHeaderComponent::outlineColourId, darkColour);
        setColour(AnanasNetworkTable::textColourId, darkColour);
        setColour(AnanasNetworkTable::okColourId, okColour);
        setColour(AnanasNetworkTable::warningColourId, warningColour);

        // Switches
        setColour(SwitchesComponent::backgroundColourId, juce::Colours::transparentWhite);

        // Time authority
        setColour(TimeAuthorityComponent::backgroundColourId, juce::Colours::transparentWhite);

        // Clients
        setColour(ClientsOverviewComponent::backgroundColourId, juce::Colours::transparentWhite);
        setColour(
            ClientsOverviewComponent::OverviewPanel::PresentationTimeInterval::okColourId,
            okColour
        );
        setColour(
            ClientsOverviewComponent::OverviewPanel::PresentationTimeInterval::warningColourId,
            warningColour
        );
        setColour(
            ClientsOverviewComponent::OverviewPanel::PresentationTimeInterval::oneMicrosecondIndicatorColourId,
            juce::Colours::white
        );
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

        auto j{juce::Justification::centred};
        if (const auto ananasTable{dynamic_cast<AnanasNetworkTable *>(header.getParentComponent()->getParentComponent())}) {
            j = ananasTable->getJustification(columnId);
        }
        g.drawFittedText(columnName, area, j, 1);
    }

    juce::Font AnanasLookAndFeel::getTextButtonFont(juce::TextButton &text_button, int buttonHeight)
    {
        return withDefaultMetrics(juce::FontOptions{juce::jmin(16.0f, static_cast<float>(buttonHeight) * 0.6f), juce::Font::bold});
    }

    void AnanasLookAndFeel::drawPresentationTimeInterval(juce::Graphics &g,
                                                         const ClientsOverviewComponent::OverviewPanel::PresentationTimeInterval &p,
                                                         const float backgroundProportion,
                                                         const float maxValue)
    {
        const auto bounds{p.getLocalBounds().toFloat()};
        const auto maxWidth{bounds.getWidth() - 1};
        const auto rectWidth{maxWidth * backgroundProportion};

        g.setColour(backgroundProportion <= getOneMicrosecondPosition()
                        ? p.findColour(ClientsOverviewComponent::OverviewPanel::PresentationTimeInterval::okColourId).withAlpha(.25f)
                        : p.findColour(ClientsOverviewComponent::OverviewPanel::PresentationTimeInterval::warningColourId).withAlpha(.25f));
        g.fillRect(0.0f, 0.0f, rectWidth, bounds.getHeight());
        if (backgroundProportion > getOneMicrosecondPosition()) {
            g.setColour(p.findColour(ClientsOverviewComponent::OverviewPanel::PresentationTimeInterval::oneMicrosecondIndicatorColourId));
            g.drawVerticalLine(maxWidth * .2f, 0.f, bounds.getHeight());
        }

        if (maxValue > 0.f) {
            g.setColour(maxValue <= getOneMicrosecondPosition()
                            ? p.findColour(ClientsOverviewComponent::OverviewPanel::PresentationTimeInterval::okColourId).withAlpha(.5f)
                            : p.findColour(ClientsOverviewComponent::OverviewPanel::PresentationTimeInterval::warningColourId).withAlpha(.5f));
            g.fillRect(maxWidth * maxValue - 1.f, 0.0f, getPresentationTimeIntervalPersistentMarkerWidth(), bounds.getHeight());
        }
    }
}
