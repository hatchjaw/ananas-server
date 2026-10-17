#ifndef SOUNDFIELDCOMPONENT_H
#define SOUNDFIELDCOMPONENT_H

#include <juce_gui_basics/juce_gui_basics.h>


namespace ananas::HOA::UI
{
    class SoundFieldComponent final : public juce::Component
    {
        public:

        //======================================================================

        enum ColourIds
        {
            backgroundColourId = 0x2002000,
            azymuthalLineColourId = 0x2002001,
        };

        //======================================================================

        void paint (juce::Graphics& g) override;
    };
}


#endif //SOUNDFIELDCOMPONENT_H
