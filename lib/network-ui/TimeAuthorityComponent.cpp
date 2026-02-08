#include "TimeAuthorityComponent.h"
#include <AnanasUtils.h>

namespace ananas::UI
{
    TimeAuthorityComponent::TimeAuthorityComponent(juce::ValueTree &treeToListenTo)
        : tree(treeToListenTo)
    {
        addAndMakeVisible(title);
        addAndMakeVisible(authorityTable);

        title.setColour(juce::Label::textColourId, juce::Colours::black);
        title.setFont(juce::Font(juce::FontOptions(15.f, juce::Font::bold)));
        title.setJustificationType(juce::Justification::centredLeft);
        title.setText(Strings::TimeAuthoritySectionTitle, juce::dontSendNotification);

        tree.addListener(this);
    }

    TimeAuthorityComponent::~TimeAuthorityComponent()
    {
        tree.removeListener(this);
    }

    void TimeAuthorityComponent::update(const juce::var &var)
    {
        authorityTable.update(var);
    }

    void TimeAuthorityComponent::paint(juce::Graphics &g)
    {
        g.fillAll(juce::Colours::transparentWhite);
    }

    void TimeAuthorityComponent::resized()
    {
        auto bounds{getLocalBounds()};
        title.setBounds(
            bounds.removeFromTop(Dimensions::NetworkSectionTitleHeight)
            .reduced(6, 0)
        );
        authorityTable.setBounds(bounds);
    }

    void TimeAuthorityComponent::valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyHasChanged, const juce::Identifier &property)
    {
        if (!isVisible()) return;

        if (property == Utils::Identifiers::TimeAuthorityParamID) {
            update(treeWhosePropertyHasChanged[property]);
            handleAsyncUpdate();
        }
    }

    void TimeAuthorityComponent::handleAsyncUpdate()
    {
        repaint();
    }

    //==============================================================================

    TimeAuthorityComponent::TimeAuthorityTable::TimeAuthorityTable()
    {
        addAndMakeVisible(table);

        addColumn(TableColumns::AuthorityTableIpAddress);
        addColumn(TableColumns::AuthorityTableSerialNumber);
        addColumn(TableColumns::AuthorityTableFeedbackAccumulator);
        addColumn(TableColumns::AuthorityTableSamplingRate);

        setLookAndFeel(&lookAndFeel);

        table.setModel(this);
        table.setOutlineThickness(1);
    }

    TimeAuthorityComponent::TimeAuthorityTable::~TimeAuthorityTable()
    {
        setLookAndFeel(nullptr);
    }

    void TimeAuthorityComponent::TimeAuthorityTable::update(const juce::var &var)
    {
        if (!isVisible()) return;

        const auto object{var.getDynamicObject()};

        row.ip = object->getProperty(Utils::Identifiers::AuthorityIpPropertyID);
        row.serialNumber = object->getProperty(Utils::Identifiers::AuthoritySerialNumberPropertyID);

        const auto feedbackAccumulator{static_cast<int>(object->getProperty(Utils::Identifiers::AuthorityFeedbackAccumulatorPropertyID))},
                feedbackAccumulatorDiff{feedbackAccumulator - Utils::Constants::AuthorityInitialUSBFeedbackAccumulator};

        row.feedbackAccumulator = juce::String{feedbackAccumulator} + (feedbackAccumulatorDiff >= 0 ? " (+" : " (") + juce::String{feedbackAccumulatorDiff} +
                                  ")";

        row.samplingRate = Strings::formatWithThousandsSeparator(1000 * static_cast<float>(feedbackAccumulator >> 12) / static_cast<float>(1 << 13)) +
                           " (0x" + juce::String{feedbackAccumulator >> 28} + "." +
                           juce::String::toHexString(feedbackAccumulator >> 12 & 0xFFFF).paddedLeft('0', 4) + ")";

        table.updateContent();
        repaint();
    }

    int TimeAuthorityComponent::TimeAuthorityTable::getNumRows()
    {
        return 1;
    }

    void TimeAuthorityComponent::TimeAuthorityTable::paintRowBackground(juce::Graphics &g, int rowNumber, int width, int height, bool rowIsSelected)
    {
        juce::ignoreUnused(width, height, rowNumber, rowIsSelected);

        g.fillAll(juce::Colours::white);
    }

    void TimeAuthorityComponent::TimeAuthorityTable::paintCell(juce::Graphics &g, int rowNumber, const int columnId, const int width, const int height,
                                                               bool rowIsSelected)
    {
        juce::ignoreUnused(rowIsSelected, rowNumber);

        const auto &[ip, serialNumber, feedbackAccumulator, samplingRate] = row;
        juce::String text;

        switch (columnId) {
            case 1: text = ip;
                break;
            case 2: text = serialNumber;
                break;
            case 3: text = feedbackAccumulator;
                break;
            case 4: text = samplingRate;
                break;
            default: break;
        }

        g.drawText(text, 2, 0, width - 4, height, getJustification(columnId), true);
    }

    void TimeAuthorityComponent::TimeAuthorityTable::resized()
    {
        table.setBounds(getLocalBounds().reduced(10));
    }

    juce::Justification TimeAuthorityComponent::TimeAuthorityTable::LookAndFeel::getTableHeaderJustification(int columnId)
    {
        switch (columnId) {
            case 1: return TableColumns::AuthorityTableIpAddress.justification;
            case 2: return TableColumns::AuthorityTableSerialNumber.justification;
            case 3: return TableColumns::AuthorityTableFeedbackAccumulator.justification;
            case 4: return TableColumns::AuthorityTableSamplingRate.justification;
            default: return AnanasLookAndFeel::getTableHeaderJustification(columnId);
        }
    }
}
