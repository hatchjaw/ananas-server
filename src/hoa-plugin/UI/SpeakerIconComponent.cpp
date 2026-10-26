#include "SpeakerIconComponent.h"

#include "HoaLookAndFeel.h"
#include "SpeakerCoordinatesComponent.h"

namespace ananas::HOA::UI
{
    SpeakerIconComponent::SpeakerIconComponent(const size_t idx,
                                               juce::AudioProcessorValueTreeState &apvts)
        : index(idx),
          state(apvts)
    {
    }

    void SpeakerIconComponent::paint(juce::Graphics &g)
    {
        if (const auto *lnf{dynamic_cast<HoaLookAndFeel *>(&getLookAndFeel())})
            lnf->drawSpeakerIcon(g, *this);
    }

    float SpeakerIconComponent::getRotation() const
    {
        switch (index) {
            case 1:
                return 0.0f;
            case 2:
                return juce::MathConstants<float>::pi / 4.f;
            case 3:
                return juce::MathConstants<float>::pi / 2.f;
            case 4:
                return 3.f * juce::MathConstants<float>::pi / 4.f;
            case 5:
                return juce::MathConstants<float>::pi;
            case 6:
                return 5.f * juce::MathConstants<float>::pi / 4.f;
            case 7:
                return 3.f * juce::MathConstants<float>::pi / 2.f;
            case 8:
                return 7.f * juce::MathConstants<float>::pi / 4.f;
            case 9:
                return 15.f * juce::MathConstants<float>::pi / 8.f;
            case 10:
                return juce::MathConstants<float>::pi / 8.f;
            case 11:
                return 3.f * juce::MathConstants<float>::pi / 8.f;
            case 12:
                return 5.f * juce::MathConstants<float>::pi / 8.f;
            case 13:
                return 7.f * juce::MathConstants<float>::pi / 8.f;
            case 14:
                return 9.f * juce::MathConstants<float>::pi / 8.f;
            case 15:
                return 11.f * juce::MathConstants<float>::pi / 8.f;
            case 16:
                return 13.f * juce::MathConstants<float>::pi / 8.f;
            default:
                return 0.f;
        }
    }

    juce::Rectangle<int> SpeakerIconComponent::getTheBounds() const
    {
        switch (index) {
            case 1:
                return {560, 0, 80, 80};
            case 2:
                return {838, 116, 80, 80};
            case 3:
                return {954, 400, 80, 80};
            case 4:
                return {838, 673, 80, 80};
            case 5:
                return {560, 790, 80, 80};
            case 6:
                return {280, 673, 80, 80};
            case 7:
                return {166, 400, 80, 80};
            case 8:
                return {280, 115, 80, 80};
            case 9:
                return {470, 170, 80, 80};
            case 10:
                return {650, 170, 80, 80};
            case 11:
                return {785, 310, 80, 80};
            case 12:
                return {782, 490, 80, 80};
            case 13:
                return {650, 618, 80, 80};
            case 14:
                return {470, 618, 80, 80};
            case 15:
                return {335, 490, 80, 80};
            case 16:
                return {335, 305, 80, 80};
            default:
                return {560, 400, 80, 80};
        }
    }

    void SpeakerIconComponent::mouseEnter(const juce::MouseEvent &event)
    {
        ignoreUnused(event);
        isHover = true;
        repaint();
    }

    void SpeakerIconComponent::mouseExit(const juce::MouseEvent &event)
    {
        ignoreUnused(event);
        isHover = false;
        repaint();
    }

    void SpeakerIconComponent::mouseUp(const juce::MouseEvent &event)
    {
        ignoreUnused(event);
        isHover = false;
        repaint();

        auto *content = new SpeakerCoordinatesComponent(index, state); // your custom component
        content->setSize(400, 300);

        juce::DialogWindow::LaunchOptions options;
        options.content.setOwned(content);
        options.dialogTitle = "Speaker " + juce::String{index} + " co-ordinates";
        options.dialogBackgroundColour = juce::Colours::transparentWhite;
        options.escapeKeyTriggersCloseButton = true;
        options.useNativeTitleBar = true;
        options.resizable = false;

        options.launchAsync(); // shows the dialog window
    }
}
