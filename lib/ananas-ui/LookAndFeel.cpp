#include "LookAndFeel.h"

namespace ananas::UI
{
    AnanasLookAndFeel::AnanasLookAndFeel()
    {
        // Background
        setColour(juce::ResizableWindow::backgroundColourId, juce::Colours::ghostwhite);

        // Text buttons
        setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::deepskyblue);

        // XY controller
        setColour(WFS::UI::XYControllerComponent::backgroundColourId, juce::Colours::black.withAlpha(0.1f));
        setColour(WFS::UI::XYControllerComponent::gridLineColourId, juce::Colours::red);

        // XY controller nodes
        setColour(WFS::UI::XYControllerComponent::Node::backgroundColourId, juce::Colours::white.withAlpha(0.5f));
        setColour(WFS::UI::XYControllerComponent::Node::borderColourId, juce::Colours::lightseagreen);
        setColour(WFS::UI::XYControllerComponent::Node::textColourId, juce::Colours::black);
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

    void AnanasLookAndFeel::drawXYController(juce::Graphics &g, const WFS::UI::XYControllerComponent &xy)
    {
        g.fillAll(xy.findColour(WFS::UI::XYControllerComponent::backgroundColourId));

        g.setColour(xy.findColour(WFS::UI::XYControllerComponent::gridLineColourId));

        const auto bounds{xy.getBounds().toFloat()};
        const auto right{bounds.getRight()};

        if (xy.getGridSpacingX() > 0) {
            // Draw positive x grid lines, one per metre
            for (int x{xy.getWidth() / 2}; x < xy.getWidth(); x += xy.getGridSpacingX()) {
                g.drawVerticalLine(x, 0.f, bounds.getHeight());
            }

            // Draw negative x grid lines
            for (int x{xy.getWidth() / 2 - xy.getGridSpacingX()}; x > 0; x -= xy.getGridSpacingX()) {
                g.drawVerticalLine(x, 0.f, bounds.getHeight());
            }
        }

        const auto yZero{xy.getMaxYMetres() * xy.getHeight() / (xy.getMaxYMetres() - xy.getMinYMetres())};
        const auto unit{xy.getHeight() / (xy.getMaxYMetres() - xy.getMinYMetres())};

        // y gridlines
        for (int y{xy.getMinYMetres()}; y < xy.getMaxYMetres(); ++y) {
            g.drawHorizontalLine(yZero - y * unit, 0.f, right);
        }
    }

    float AnanasLookAndFeel::getNodeBorderBrightnessMin()
    {
        return .2f;
    }

    float AnanasLookAndFeel::getNodeBorderBrightnessMax()
    {
        return .9f;
    }

    float AnanasLookAndFeel::getNodeBorderSaturationRange()
    {
        return .9f;
    }

    float AnanasLookAndFeel::getNodeBorderAlpha()
    {
        return .75f;
    }

    float AnanasLookAndFeel::getNodeBorderThickness()
    {
        return 3.f;
    }

    float AnanasLookAndFeel::getNodeIndexFontSize()
    {
        return 20.f;
    }

    void AnanasLookAndFeel::drawXYControllerNode(juce::Graphics &g, const WFS::UI::XYControllerComponent::Node &node, const float intensity)
    {
        const auto bounds{node.getLocalBounds().toFloat()};

        // Background
        g.setColour(node.findColour(WFS::UI::XYControllerComponent::Node::ColourIds::backgroundColourId));
        g.fillEllipse(bounds);

        // Border
        const auto level{(100.f + intensity) * .01f};
        const auto baseBorderColour{node.findColour(WFS::UI::XYControllerComponent::Node::ColourIds::borderColourId)};

        const auto saturation{1.0f - level * getNodeBorderSaturationRange()};
        const auto brightness{getNodeBorderBrightnessMin() + level * getNodeBorderBrightnessMax()};

        g.setColour(juce::Colour::fromHSV(
            baseBorderColour.getHue(),
            saturation,
            brightness,
            getNodeBorderAlpha()));

        const auto borderThickness{getNodeBorderThickness()};
        g.drawEllipse(bounds.withSizeKeepingCentre(
                          bounds.getWidth() - borderThickness,
                          bounds.getHeight() - borderThickness),
                      borderThickness);

        // Index
        g.setColour(node.findColour(WFS::UI::XYControllerComponent::Node::ColourIds::textColourId));
        g.setFont(getNodeIndexFontSize());
        g.drawText(juce::String(node.getIndex() + 1), node.getLocalBounds(),
                   juce::Justification::centred);
    }
}
