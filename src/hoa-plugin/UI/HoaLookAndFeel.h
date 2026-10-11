#ifndef HOALOOKANDFEEL_H
#define HOALOOKANDFEEL_H

#include <AnanasLookAndFeel.h>
#include <Packet.h>
#include "SoundFieldComponent.h"
#include "SpeakerIconComponent.h"

namespace ananas::HOA::UI
{
    class HoaLookAndFeel final : public ananas::UI::AnanasLookAndFeel<HoaClientAnnouncePacket>
    {
    public:
        HoaLookAndFeel();

        void drawSoundField(juce::Graphics &g, const SoundFieldComponent &s) const;

        void drawSpeakerIcon(juce::Graphics &g, const SpeakerIconComponent &s) const;

    private:
        [[nodiscard]] virtual float getAzimuthalLineThickness() const { return 2.f; }
        [[nodiscard]] virtual float getFirstAzimuthalRingReduction() const { return 50.f; }
        [[nodiscard]] virtual float getSecondAzimuthalRingReduction() const { return 200.f; }

        [[nodiscard]] juce::Path createSpeakerIconPath() const;

        [[nodiscard]] virtual float getSpeakerOutlineThickness() const { return 1.f; };
    };
}


#endif //HOALOOKANDFEEL_H
