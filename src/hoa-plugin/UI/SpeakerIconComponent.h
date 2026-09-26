#ifndef SPEAKERICONCOMPONENT_H
#define SPEAKERICONCOMPONENT_H
#include <juce_gui_basics/juce_gui_basics.h>

namespace ananas::HOA::UI
{
    class SpeakerIconComponent final : public juce::Component
    {
    public:
        explicit SpeakerIconComponent(int index);

        //==================================================================

        enum ColourIds
        {
            fillColourId = 0x2002100,
            borderColourId = 0x2002101
        };

        //==================================================================

        void paint(juce::Graphics &g) override;

        [[nodiscard]] float getRotation() const;

        juce::Rectangle<int> getTheBounds() const;

    private:
        int index{0};
    };
}

#endif //SPEAKERICONCOMPONENT_H
