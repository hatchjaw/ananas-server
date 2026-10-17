#ifndef ANANASUILOOKANDFEEL_H
#define ANANASUILOOKANDFEEL_H

#include <ClientsOverviewComponent.h>
#include <OverlayableComponent.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace ananas::UI
{
    class AnanasLookAndFeelBase : public juce::LookAndFeel_V4
    {
    public:
        AnanasLookAndFeelBase();

        juce::Typeface::Ptr getTypefaceForFont(const juce::Font &) override;

        int getTabButtonBestWidth(juce::TabBarButton &, int tabDepth) override;

        void setTotalWidth(int width);

        void setNumberOfTabs(int numTabs);

        void drawOverlay(juce::Graphics &g, const OverlayableComponent::OverlayComponent &o, const juce::String &text) const;

        void drawTableHeaderColumn(
            juce::Graphics &,
            juce::TableHeaderComponent &,
            const juce::String &columnName,
            int columnId,
            int width,
            int height,
            bool isMouseOver,
            bool isMouseDown,
            int columnFlags
        ) override;

        void drawButtonBackground(juce::Graphics &, juce::Button &, const juce::Colour &backgroundColour, bool shouldDrawButtonAsHighlighted,
                                  bool shouldDrawButtonAsDown) override;

        juce::Font getTextButtonFont(juce::TextButton &, int buttonHeight) override;

        [[nodiscard]] virtual float getMaxMicrosecondsToDisplay() const { return 10.f; }

    protected:
        [[nodiscard]] virtual float getOneMicrosecondProportion() const { return 1.f / getMaxMicrosecondsToDisplay(); }

        [[nodiscard]] virtual float getPresentationTimeIntervalPersistentMarkerWidth() const { return 2.f; }

        juce::Typeface::Ptr typefaceSans, typefaceBold;
        juce::Colour darkColour, lightColour, okColour, warningColour;

    private:
        [[nodiscard]] virtual int getOverlayBoxWidth() const { return 400; }

        [[nodiscard]] virtual int getOverlayBoxHeight() const { return 200; }

        [[nodiscard]] virtual float getOverlayBoxBorderRadius() const { return 10.f; }

        [[nodiscard]] virtual float getOverlayBoxBorderThickness() const { return 5.f; }

        [[nodiscard]] virtual int getOverlayBoxShadowOffset() const { return 4; }

        [[nodiscard]] virtual float getOverlayBoxTextSize() const { return 20.f; }

        int totalWidth{};
        int totalNumTabs{};
    };

    //==========================================================================

    template<typename AnnouncePacketType>
    class AnanasLookAndFeel : public AnanasLookAndFeelBase
    {
    public:
        AnanasLookAndFeel();

        void drawPresentationTimeInterval(juce::Graphics &g,
                                          const typename ClientsOverviewComponent<AnnouncePacketType>::OverviewPanel::PresentationTimeInterval &p,
                                          float backgroundProportion,
                                          float maxValue) const;
    };
}

#endif //ANANASUILOOKANDFEEL_H
