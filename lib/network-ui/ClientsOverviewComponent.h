#ifndef CLIENTSOVERVIEWCOMPONENT_H
#define CLIENTSOVERVIEWCOMPONENT_H

#include <AnanasLookAndFeel.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "AnanasNetworkTable.h"

namespace ananas::UI
{
    class ClientsOverviewComponent final : public juce::Component,
                                           public juce::ValueTree::Listener,
                                           juce::AsyncUpdater
    {
    public:
        explicit ClientsOverviewComponent(juce::ValueTree &treeToListenTo);

        ~ClientsOverviewComponent() override;

        //======================================================================

        enum ColourIds
        {
            backgroundColourId = 0x2001400
        };

        //======================================================================

        void update(const juce::var &var);

        void paint(juce::Graphics &g) override;

        void resized() override;

        void valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyHasChanged, const juce::Identifier &property) override;

        void handleAsyncUpdate() override;

        void triggerClientReboot() const;

    private:
        class OverviewPanel final : public Component
        {
        public:
            OverviewPanel();

            juce::String getPresentationTimeIntervalText(int presentationTimeInterval);

            void update(const juce::var &var);

            void paint(juce::Graphics &g) override;

            void resized() override;

        private:
            class PresentationTimeInterval final : public juce::Label
            {
            public:
                void setBackgroundWidth(float proportion);

                void setMax(float maxSeen);

                void paint(juce::Graphics &g) override;

            private:
                float backgroundProportion{0.f};
                float maxValue{0.f};
            };

            juce::Label totalClientsLabel;
            juce::Label totalClientsValue;
            juce::Label presentationTimeIntervalLabel;
            PresentationTimeInterval presentationTimeIntervalValue;
            juce::int32 maxPresentationTimeInterval{0};
            uint numUpdates{0};
        };

        class ClientTable final : public AnanasNetworkTable
        {
        public:
            ClientTable();

            ~ClientTable() override;

            void update(const juce::var &clientInfo);

            int getNumRows() override;

            void paintRowBackground(juce::Graphics &g, int rowNumber, int width,
                                    int height, bool rowIsSelected) override;

            void paintCell(juce::Graphics &g, int rowNumber, int columnId,
                           int width, int height, bool rowIsSelected) override;

            void resized() override;

        private:
            class LookAndFeel final : public AnanasLookAndFeel
            {
            protected:
                juce::Justification getTableHeaderJustification(int columnId) override;
            };

            struct Row
            {
                juce::String ip;
                juce::String serialNumber;
                bool ptpLock;
                juce::int32 presentationTimeOffsetNs;
                juce::int32 presentationTimeOffsetFrame;
                juce::int32 audioPTPOffsetNs;
                juce::int32 bufferFillPercent;
                float samplingRate;
                float percentCPU;
                juce::int32 moduleID;
            };

            juce::Array<Row> rows;
            LookAndFeel lookAndFeel;
        };

        juce::Label title;
        juce::TextButton rebootAllClientsButton;
        OverviewPanel overviewPanel;
        ClientTable clientTable;
        juce::ValueTree &tree;
    };
}


#endif //CLIENTSOVERVIEWCOMPONENT_H
