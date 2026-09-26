#ifndef WFSLOOKANDFEEL_H
#define WFSLOOKANDFEEL_H

#include <AnanasLookAndFeel.h>
#include "SpeakerIconComponent.h"
#include "XYControllerComponent.h"

namespace ananas::WFS::UI
{
    class WfsLookAndFeel : public ananas::UI::AnanasLookAndFeel
    {
    public:
        WfsLookAndFeel();

        void drawXYController(juce::Graphics &g, const XYControllerComponent &xy);

        void drawXYControllerNode(juce::Graphics &g, const XYControllerComponent::Node &node, float intensity) const;

        void drawSpeakerIcon(juce::Graphics &g, const SpeakerIconComponent &s) const;

    private:
        [[nodiscard]] juce::Path createSpeakerIconPath() const;

        [[nodiscard]] virtual float getSpeakerIconCoilStartX() const { return 26.f; }

        [[nodiscard]] virtual float getSpeakerIconCoilStartY() const { return 0.f; }

        [[nodiscard]] virtual float getSpeakerIconCoilWidth() const { return 48.f; }

        [[nodiscard]] virtual float getSpeakerIconConeEndY() const { return 100.f; }

        [[nodiscard]] virtual float getSpeakerIconConeRightX() const { return 100.f; }

        [[nodiscard]] virtual float getSpeakerIconConeLeftX() const { return 0.f; }

        [[nodiscard]] virtual float getSpeakerIconCoilHeight() const { return 25.f; }

        [[nodiscard]] virtual float getSpeakerOutlineThickness() const { return 0.f; }

        virtual float getNodeBorderBrightnessMin() const { return .2f; }

        virtual float getNodeBorderBrightnessMax() const { return .9f; }

        virtual float getNodeBorderSaturationRange() const { return .9f; }

        virtual float getNodeBorderAlpha() const { return .75f; }

        virtual float getNodeBorderThickness() const { return 3.f; }

        virtual float getNodeIndexFontSize() const { return 18.f; }
    };
}

#endif //WFSLOOKANDFEEL_H
