#ifndef SPEAKERICONCOMPONENT_H
#define SPEAKERICONCOMPONENT_H

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace ananas::HOA::UI
{
    class SpeakerIconComponent final : public juce::Component
    {
    public:
        SpeakerIconComponent(size_t idx,
                             juce::AudioProcessorValueTreeState &apvts);

        //==================================================================

        enum ColourIds
        {
            fillColourId = 0x2002100,
            mouseOverFillColourId = 0x2002101,
            borderColourId = 0x2002102
        };

        //==================================================================

        void paint(juce::Graphics &g) override;

        [[nodiscard]] float getRotation() const;

        [[nodiscard]] juce::Rectangle<int> getTheBounds() const;

        void mouseEnter(const juce::MouseEvent &event) override;

        void mouseExit(const juce::MouseEvent &event) override;

        void mouseUp(const juce::MouseEvent &event) override;

    private:
        size_t index{0};
        juce::AudioProcessorValueTreeState &state;
    };
}

#endif //SPEAKERICONCOMPONENT_H
