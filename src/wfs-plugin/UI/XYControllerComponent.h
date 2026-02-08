#ifndef XYCONTROLLERCOMPONENT_H
#define XYCONTROLLERCOMPONENT_H

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

namespace ananas::WFS::UI
{
    class XYControllerComponent final : public juce::Component,
                                        public juce::AudioProcessorValueTreeState::Listener,
                                        public juce::Timer
    {
    public:
        XYControllerComponent(int numNodesToCreate,
                              juce::AudioProcessorValueTreeState &apvts,
                              juce::HashMap<int, std::atomic<float> *> &sourceAmplitudes);

        //======================================================================

        enum ColourIds
        {
            backgroundColourId = 0x1001000,
            gridLineColourId = 0x1001001,
        };

        //======================================================================

        void paint(juce::Graphics &g) override;

        void resized() override;

        void calculateGridSpacingX();

        float getSpeakerArrayWidth() const;

        void parameterChanged(const juce::String &parameterID, float newValue) override;

        void mouseDown(const juce::MouseEvent &event) override;

        void hideAllNodesBesides(int nodeIdNotToHide);

        void timerCallback() override;

        int getGridSpacingX() const;

        int getMaxYMetres() const;

        int getMinYMetres() const;

        class Node final : public Component,
                           public juce::AsyncUpdater,
                           public juce::SettableTooltipClient
        {
        public:
            class Listener;

            explicit Node(int idx);

            //======================================================================

            enum ColourIds
            {
                backgroundColourId = 0x1002000,
                borderColourId = 0x1002001,
                textColourId = 0x1002002,
            };

            //======================================================================

            void paint(juce::Graphics &g) override;

            void mouseDown(const juce::MouseEvent &event) override;

            void hide();

            void mouseDrag(const juce::MouseEvent &event) override;

            void mouseUp(const juce::MouseEvent &event) override;

            void setValueX(float newX, juce::NotificationType notification);

            void setValueY(float newY, juce::NotificationType notification);

            void setBounds();

            void handleAsyncUpdate() override;

            [[nodiscard]] juce::Point<float> getValue() const;

            void sendDragStart();

            void sendDragEnd();

            void updateTooltip();

            void addListener(Listener *listener);

            void removeListener(Listener *listener);

            int getIndex() const;

            void setIntensity(float newIntensity);

            class Listener
            {
            public:
                virtual ~Listener() = default;

                virtual void valueChanged(Node *) = 0;

                virtual void dragStarted(Node *) = 0;

                virtual void dragEnded(Node *) = 0;
            };

            class ScopedDragNotification
            {
            public:
                explicit ScopedDragNotification(Node &node);

                ~ScopedDragNotification();

            private:
                Node &nodeBeingDragged;

                JUCE_DECLARE_NON_MOVEABLE(ScopedDragNotification)

                JUCE_DECLARE_NON_COPYABLE(ScopedDragNotification)
            };

        private:
            void triggerChangeMessage(juce::NotificationType notification);

            int index{};
            juce::Point<float> value{};
            juce::ListenerList<Listener> listeners;
            std::unique_ptr<ScopedDragNotification> currentDrag;
            float intensity{-100.f};
        };

    private:
        class ParameterAttachment final : Node::Listener
        {
        public:
            ParameterAttachment(uint sourceIndex,
                                const juce::AudioProcessorValueTreeState &state,
                                Node &n,
                                juce::UndoManager *um = nullptr);

            ~ParameterAttachment() override;

        private:
            void setValueX(float newX);

            void setValueY(float newY);

            void valueChanged(Node *node) override;

            void dragStarted(Node *node) override;

            void dragEnded(Node *node) override;

            Node &node;
            juce::ParameterAttachment attachmentX, attachmentY;
            bool ignoreCallbacks{false};
        };

        class Attachment
        {
        public:
            Attachment(int sourceIndex,
                       juce::AudioProcessorValueTreeState &state,
                       Node &node);

        private:
            std::unique_ptr<ParameterAttachment> attachment;
            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Attachment);
        };

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(XYControllerComponent)

        juce::OwnedArray<Node> nodes;
        juce::OwnedArray<Attachment> attachments;
        int xGridSpacing{};
        juce::AudioProcessorValueTreeState &state;
        juce::HashMap<int, std::atomic<float> *> &nodeIntensities;
    };
} // ananas::WFS

#endif //XYCONTROLLERCOMPONENT_H
