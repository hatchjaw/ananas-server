#ifndef HOALOOKANDFEEL_H
#define HOALOOKANDFEEL_H

#include <AnanasLookAndFeel.h>

#include "SoundFieldComponent.h"
#include "SpeakerIconComponent.h"

namespace ananas::HOA::UI
{
    class HoaLookAndFeel : public ananas::UI::AnanasLookAndFeel
    {
    public:
        HoaLookAndFeel();

        void drawSoundField(juce::Graphics &g, const SoundFieldComponent &s) const;


        void drawSpeakerIcon(juce::Graphics &g, const SpeakerIconComponent &s) const;

    private:
        virtual float getAzymuthalLineThickness() const { return 2.f; }
        virtual float getFirstAzymuthalRingReduction() const { return 50.f; }
        virtual float getSecondAzymuthalRingReduction() const { return 200.f; }

        juce::Path createSpeakerIconPath() const;

        virtual float getSpeakerOutlineThickness() const { return 0.f; };
    };
}


#endif //HOALOOKANDFEEL_H
