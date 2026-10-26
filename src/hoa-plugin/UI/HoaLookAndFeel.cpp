#include "HoaLookAndFeel.h"

namespace ananas::HOA::UI
{
    HoaLookAndFeel::HoaLookAndFeel()
    {
        // Sound field
        setColour(SoundFieldComponent::backgroundColourId, juce::Colours::black.withAlpha(0.1f));
        setColour(SoundFieldComponent::azimuthalLineColourId, juce::Colours::black.withAlpha(0.25f));

        // Speaker icons
        setColour(SpeakerIconComponent::fillColourId, juce::Colours::lightgrey);
        setColour(SpeakerIconComponent::mouseOverFillColourId, juce::Colours::white);
        setColour(SpeakerIconComponent::borderColourId, juce::Colours::darkgrey);
    }

    void HoaLookAndFeel::drawSoundField(juce::Graphics &g, const SoundFieldComponent &s) const
    {
        g.fillAll(s.findColour(SoundFieldComponent::backgroundColourId));

        g.setColour(s.findColour(SoundFieldComponent::azimuthalLineColourId));

        const auto bounds{s.getLocalBounds().toFloat()};
        const auto diameter{juce::jmin(bounds.getWidth(), bounds.getHeight())};
        auto circleBounds{juce::Rectangle(diameter, diameter)};
        circleBounds.setCentre(bounds.getCentre());
        g.drawEllipse(circleBounds.reduced(getFirstAzimuthalRingReduction()), getAzimuthalLineThickness());
        g.drawEllipse(circleBounds.reduced(getSecondAzimuthalRingReduction()), getAzimuthalLineThickness());
    }

    void HoaLookAndFeel::drawSpeakerIcon(juce::Graphics &g, const SpeakerIconComponent &s) const
    {
        auto speaker{createSpeakerIconPath()};

        speaker.applyTransform(juce::AffineTransform::rotation(s.getRotation(), 40, 40));

        g.setColour(s.findColour(
            s.getIsHovered() ?
            SpeakerIconComponent::mouseOverFillColourId :
            SpeakerIconComponent::fillColourId));
        g.fillPath(speaker);

        g.setColour(s.findColour(SpeakerIconComponent::borderColourId));
        g.strokePath(speaker, juce::PathStrokeType{getSpeakerOutlineThickness(), juce::PathStrokeType::mitered});
    }

    juce::Path HoaLookAndFeel::createSpeakerIconPath() const
    {
        juce::Path speaker;

        speaker.startNewSubPath(60.f, 20.f);
        speaker.lineTo(60.f, 40.f);
        speaker.lineTo(75.f, 60.f);
        speaker.lineTo(5.f, 60.f);
        speaker.lineTo(20.f, 40.f);
        speaker.lineTo(20.f, 20.f);
        speaker.closeSubPath();

        return speaker;
    }
}
