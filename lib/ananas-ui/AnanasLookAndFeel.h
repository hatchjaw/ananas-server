#ifndef ANANASUILOOKANDFEEL_H
#define ANANASUILOOKANDFEEL_H

#include <OverlayableComponent.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace ananas::UI
{
    class AnanasLookAndFeel : public juce::LookAndFeel_V4
    {
    public:
        AnanasLookAndFeel();

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

        juce::Font getTextButtonFont(juce::TextButton &, int buttonHeight) override;

    protected:
        virtual juce::Justification getTableHeaderJustification(int columnId)
        {
            juce::ignoreUnused(columnId);
            return juce::Justification::centred;
        }

        juce::Typeface::Ptr typefaceSans, typefaceBold;
        juce::Colour darkTextColour, lightTextColour, okColour, warningColour;

    private:
        virtual int getOverlayBoxWidth() const { return 400; }

        virtual int getOverlayBoxHeight() const { return 200; }

        virtual float getOverlayBoxBorderRadius() const { return 10.f; }

        virtual float getOverlayBoxBorderThickness() const { return 5.f; }

        virtual int getOverlayBoxShadowOffset() const { return 4; }

        virtual float getOverlayBoxTextSize() const { return 20.f; }

        int totalWidth{};
        int totalNumTabs{};
    };
}

#endif //ANANASUILOOKANDFEEL_H
