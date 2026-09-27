#ifndef SPEAKERCOORDINATESCOMPONENT_H
#define SPEAKERCOORDINATESCOMPONENT_H

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace ananas::HOA::UI
{
    class SpeakerCoordinatesComponent : public juce::Component
    {
    public:
        SpeakerCoordinatesComponent(size_t idx,
                                    juce::AudioProcessorValueTreeState &apvts);

        void paint(juce::Graphics &g) override;

        void resized() override;

    private:
        juce::Slider xSlider;
        juce::Label xLabel;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> xAttachment;
        juce::Slider ySlider;
        juce::Label yLabel;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> yAttachment;
        juce::Slider zSlider;
        juce::Label zLabel;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> zAttachment;
    };
}

#endif //SPEAKERCOORDINATESCOMPONENT_H
