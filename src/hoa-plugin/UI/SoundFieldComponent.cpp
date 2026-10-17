#include "SoundFieldComponent.h"

#include "HoaLookAndFeel.h"

namespace ananas::HOA::UI
{
    void SoundFieldComponent::paint(juce::Graphics &g)
    {
        if (auto *lnf{dynamic_cast<HoaLookAndFeel *>(&getLookAndFeel())})
            lnf->drawSoundField(g, *this);
    }
}
