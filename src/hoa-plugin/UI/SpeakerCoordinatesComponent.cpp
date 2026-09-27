#include "SpeakerCoordinatesComponent.h"

#include <juce_audio_processors/juce_audio_processors.h>

#include "../HOAUtils.h"

namespace ananas::HOA::UI
{
    SpeakerCoordinatesComponent::SpeakerCoordinatesComponent(const size_t idx,
                                                             juce::AudioProcessorValueTreeState &apvts)
    {
        addAndMakeVisible(xLabel);
        xLabel.attachToComponent(&xSlider, true);
        xLabel.setText("X", juce::dontSendNotification);
        xLabel.setJustificationType(juce::Justification::left);
        addAndMakeVisible(xSlider);
        xAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts,
            Params::getSpeakerCoordinateParamID(idx, SpeakerCoordinateAxis::X),
            xSlider
        );

        addAndMakeVisible(yLabel);
        yLabel.attachToComponent(&ySlider, true);
        yLabel.setText("Y", juce::dontSendNotification);
        yLabel.setJustificationType(juce::Justification::left);
        addAndMakeVisible(ySlider);
        yAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts,
            Params::getSpeakerCoordinateParamID(idx, SpeakerCoordinateAxis::Y),
            ySlider
        );

        addAndMakeVisible(zLabel);
        zLabel.attachToComponent(&zSlider, true);
        zLabel.setText("Z", juce::dontSendNotification);
        zLabel.setJustificationType(juce::Justification::left);
        addAndMakeVisible(zSlider);
        zAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts,
            Params::getSpeakerCoordinateParamID(idx, SpeakerCoordinateAxis::Z),
            zSlider
        );
    }

    void SpeakerCoordinatesComponent::paint(juce::Graphics &g)
    {
        g.fillAll(juce::Colours::transparentWhite);
    }

    void SpeakerCoordinatesComponent::resized()
    {
        juce::Grid grid;
        grid.templateRows = { juce::Grid::TrackInfo(juce::Grid::Fr(1)),
                          juce::Grid::TrackInfo(juce::Grid::Fr(1)),
                          juce::Grid::TrackInfo(juce::Grid::Fr(1)) }; // 3 rows
        grid.templateColumns = { juce::Grid::TrackInfo(juce::Grid::Fr(1)),
                                 juce::Grid::TrackInfo(juce::Grid::Fr(5)) }; // label col, slider col

        grid.items.add(juce::GridItem(xLabel));
        grid.items.add(juce::GridItem(xSlider));
        grid.items.add(juce::GridItem(yLabel));
        grid.items.add(juce::GridItem(ySlider));
        grid.items.add(juce::GridItem(zLabel));
        grid.items.add(juce::GridItem(zSlider));

        grid.performLayout(getLocalBounds());
    }
}
