#include "SpeakerIconComponent.h"

#include "HoaLookAndFeel.h"

namespace ananas::HOA::UI
{
    SpeakerIconComponent::SpeakerIconComponent(const int index) : index(index)
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
            case 0:
                return 0.0f;
            case 1:
                return juce::MathConstants<float>::pi / 4.f;
            case 2:
                return juce::MathConstants<float>::pi / 2.f;
            case 3:
                return 3.f * juce::MathConstants<float>::pi / 4.f;
            case 4:
                return juce::MathConstants<float>::pi;
            case 5:
                return 5.f * juce::MathConstants<float>::pi / 4.f;
            case 6:
                return 3.f * juce::MathConstants<float>::pi / 2.f;
            case 7:
                return 7.f * juce::MathConstants<float>::pi / 4.f;
            case 8:
                return 15.f * juce::MathConstants<float>::pi / 8.f;
            case 9:
                return juce::MathConstants<float>::pi / 8.f;
            case 10:
                return 3.f * juce::MathConstants<float>::pi / 8.f;
            case 11:
                return 5.f * juce::MathConstants<float>::pi / 8.f;
            case 12:
                return 7.f * juce::MathConstants<float>::pi / 8.f;
            case 13:
                return 9.f * juce::MathConstants<float>::pi / 8.f;
            case 14:
                return 11.f * juce::MathConstants<float>::pi / 8.f;
            case 15:
                return 13.f * juce::MathConstants<float>::pi / 8.f;
            default:
                return 0.f;
        }
    }

    juce::Rectangle<int> SpeakerIconComponent::getTheBounds() const
    {
        switch (index) {
            case 0:
                return {560, 0, 80, 80};
            case 1:
                return {840, 115, 80, 80};
            case 2:
                return {954, 400, 80, 80};
            case 3:
                return {840, 675, 80, 80};
            case 4:
                return {560, 790, 80, 80};
            case 5:
                return {280, 675, 80, 80};
            case 6:
                return {166, 400, 80, 80};
            case 7:
                return {280, 115, 80, 80};
            case 8:
                return {460, 170, 80, 80};
            case 9:
                return {660, 170, 80, 80};
            case 10:
                return {};
            case 11:
                return {};
            case 12:
                return {};
            case 13:
                return {};
            case 14:
                return {};
            case 15:
                return {};
            default:
                return {560, 400, 80, 80};
        }
    }
}
