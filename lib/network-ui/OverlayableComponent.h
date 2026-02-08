#ifndef OVERLAYABLECOMPONENT_H
#define OVERLAYABLECOMPONENT_H

#include <juce_gui_basics/juce_gui_basics.h>

namespace ananas::UI
{
    class OverlayableComponent : public juce::Component,
                                 public juce::ChangeListener
    {
    public:
        OverlayableComponent();

        void changeListenerCallback(juce::ChangeBroadcaster *source) override;

        void resized() override;

        class OverlayComponent final : public Component
        {
        public:
            //==================================================================

            enum ColourIds
            {
                backgroundColourId = 0x2001000,
                boxShadowColourId = 0x2001001,
                boxBackgroundColourId = 0x2001002,
                boxBorderColourId = 0x2001003,
                boxTextColourId = 0x2001004,
            };

            //==================================================================

            void paint(juce::Graphics &g) override;

            void setText(const juce::String &textToDisplay);
        private:
            juce::String text;
        };

    private:
        std::unique_ptr<OverlayComponent> overlay;
    };
} // ananas

#endif //OVERLAYABLECOMPONENT_H
