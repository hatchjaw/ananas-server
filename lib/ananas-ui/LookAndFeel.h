#ifndef ANANASUILOOKANDFEEL_H
#define ANANASUILOOKANDFEEL_H

#include <juce_gui_basics/juce_gui_basics.h>

#include "../../src/wfs-plugin/UI/XYControllerComponent.h"

namespace ananas::UI
{
    class AnanasLookAndFeel final : public juce::LookAndFeel_V4
    {
    public:
        AnanasLookAndFeel();

        int getTabButtonBestWidth(juce::TabBarButton &, int tabDepth) override;

        void setTotalWidth(int width);

        void setNumberOfTabs(int numTabs);

        void drawXYController(juce::Graphics &g, const WFS::UI::XYControllerComponent &xy);

        float getNodeBorderBrightnessMin();

        float getNodeBorderBrightnessMax();

        float getNodeBorderSaturationRange();

        float getNodeBorderAlpha();

        float getNodeBorderThickness();

        float getNodeIndexFontSize();

        void drawXYControllerNode(juce::Graphics &g, const WFS::UI::XYControllerComponent::Node &node, float intensity);

    private:
        int totalWidth{};
        int totalNumTabs{};
    };
}

#endif //ANANASUILOOKANDFEEL_H
