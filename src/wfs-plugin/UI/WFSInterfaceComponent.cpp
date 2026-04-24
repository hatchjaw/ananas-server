#include "WFSInterfaceComponent.h"
#include <AnanasUtils.h>
#include <Server.h>

#include "../WFSUtils.h"

namespace ananas::WFS::UI
{
    WFSInterfaceComponent::WFSInterfaceComponent(
        const int numSources,
        juce::AudioProcessorValueTreeState &apvts,
        juce::ValueTree &persistentTreeToListenTo,
        juce::HashMap<int, std::atomic<float> *> &sourceAmplitudes
    ) : state(apvts),
        xyController(numSources, apvts, sourceAmplitudes),
        persistentTree(persistentTreeToListenTo)
    {
        // Display the XY-controller
        addAndMakeVisible(xyController);

        // Display and attach the number-of-modules selector.
        addAndMakeVisible(numModulesLabel);
        numModulesLabel.attachToComponent(&numModulesSelector, true);
        numModulesLabel.setText(Params::NumModules.name, juce::dontSendNotification);
        numModulesLabel.setJustificationType(juce::Justification::centredRight);

        addAndMakeVisible(numModulesSelector);
        for (size_t n{0}; n < Constants::MaxNumModules; ++n) {
            numModulesSelector.addItem(juce::String{n + 1}, n + 1);
        }

        numModulesAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
            state,
            Params::NumModules.id,
            numModulesSelector
        );

        // Display and attach the speaker spacing selector.
        addAndMakeVisible(speakerSpacingLabel);
        speakerSpacingLabel.attachToComponent(&speakerSpacingSlider, true);
        speakerSpacingLabel.setText(Params::SpeakerSpacing.name, juce::dontSendNotification);
        speakerSpacingLabel.setJustificationType(juce::Justification::centredRight);

        addAndMakeVisible(speakerSpacingSlider);
        speakerSpacingSlider.setSliderStyle(juce::Slider::SliderStyle::IncDecButtons);
        speakerSpacingSlider.setNormalisableRange(Params::SpeakerSpacing.rangeDouble);
        speakerSpacingSlider.setValue(Params::SpeakerSpacing.defaultValue);
        speakerSpacingSlider.setTextBoxStyle(juce::Slider::TextBoxLeft,
                                             false,
                                             speakerSpacingSlider.getTextBoxWidth(),
                                             25);
        speakerSpacingSlider.setIncDecButtonsMode(juce::Slider::incDecButtonsDraggable_Vertical);

        speakerSpacingAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            state,
            Params::SpeakerSpacing.id,
            speakerSpacingSlider
        );

        // Display and attach the show-module-selectors checkbox.
        addAndMakeVisible(showModuleSelectorsButton);
        showModuleSelectorsButton.setButtonText(Params::ShowModuleSelectors.name);

        showModuleSelectorsAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
            state,
            Params::ShowModuleSelectors.id,
            showModuleSelectorsButton
        );

        // Listen for changes to the number of modules.
        state.addParameterListener(Params::NumModules.id, this);

        // Listen for changes to the speaker spacing.
        state.addParameterListener(Params::SpeakerSpacing.id, this);

        // Listen for changes to module selector display state.
        state.addParameterListener(Params::ShowModuleSelectors.id, this);

        // Set initial module display state.
        parameterChanged(Params::NumModules.id, state.getRawParameterValue(Params::NumModules.id)->load());

        // Set initial module selector display state.
        parameterChanged(Params::ShowModuleSelectors.id, state.getRawParameterValue(Params::ShowModuleSelectors.id)->load());

        // Listen to the persistent tree for module selection changes.
        persistentTree.addListener(this);
        // Trigger an initial property change so that combo boxes get populated.
        persistentTree.sendPropertyChangeMessage(ananas::Utils::Identifiers::ModulesParamID);
    }

    WFSInterfaceComponent::~WFSInterfaceComponent()
    {
        state.removeParameterListener(Params::ShowModuleSelectors.id, this);
        state.removeParameterListener(Params::SpeakerSpacing.id, this);
        state.removeParameterListener(Params::NumModules.id, this);
        persistentTree.removeListener(this);
        showModuleSelectorsButton.setLookAndFeel(nullptr);
    }

    void WFSInterfaceComponent::paint(juce::Graphics &g)
    {
        g.fillAll(juce::Colours::transparentWhite);
    }

    void WFSInterfaceComponent::resized()
    {
        auto bounds{getLocalBounds()};
        auto optionsRow{
            bounds.removeFromTop(Dimensions::SpeakerSpacingSectionHeight)
            .reduced(6, 0)
        };
        speakerSpacingSlider.setBounds(optionsRow.removeFromRight(100).reduced(1, 3));
        speakerSpacingLabel.setBounds(optionsRow.removeFromRight(200));

        numModulesSelector.setBounds(optionsRow.removeFromRight(75).reduced(1, 12));
        numModulesLabel.setBounds(optionsRow.removeFromRight(200));

        showModuleSelectorsButton.setBounds(optionsRow.removeFromLeft(300));

        bounds = bounds.reduced(10);
        xyController.setBounds(bounds);

        const auto yZero{Constants::MaxYMetres * bounds.getHeight() / (Constants::MaxYMetres - Constants::MinYMetres)};
        bounds.removeFromTop(yZero - Dimensions::ModuleSelectorHeight - Dimensions::SpeakerIconHeight);

        juce::FlexBox moduleFlex;
        moduleFlex.flexDirection = juce::FlexBox::Direction::row;

        for (auto *m: modules) {
            moduleFlex.items.add(juce::FlexItem(*m)
                .withFlex(1.f) // Equal flex = equal width
                .withMaxHeight(Dimensions::ModuleSelectorHeight));
        }

        moduleFlex.performLayout(bounds);

        bounds.removeFromTop(Dimensions::ModuleSelectorHeight);

        juce::FlexBox speakerFlex;
        // Speaker icons
        speakerFlex.flexDirection = juce::FlexBox::Direction::row;
        speakerFlex.justifyContent = juce::FlexBox::JustifyContent::center;
        for (const auto &s: speakerIcons) {
            speakerFlex.items.add(juce::FlexItem{*s}
                .withFlex(1.f)
                .withHeight(Dimensions::SpeakerIconHeight));
        }

        speakerFlex.performLayout(bounds);

#if SHOW_NO_NETWORK_OVERLAY
        OverlayableComponent::resized();
#endif
    }

    void WFSInterfaceComponent::updateModuleLists(const juce::var &var)
    {
        juce::StringArray ips;
        if (auto *obj = var.getDynamicObject()) {
            for (const auto &prop: obj->getProperties()) {
                auto module{obj->getProperty(prop.name).getDynamicObject()};
                if (module->getProperty(ananas::Utils::Identifiers::ModuleIsConnectedPropertyID)) {
                    ips.add(prop.name.toString());
                }
            }
        }
        for (const auto &m: modules) {
            m->setAvailableModules(ips);
        }
    }

    void WFSInterfaceComponent::valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyHasChanged, const juce::Identifier &property)
    {
        // if (!isVisible()) return;

        if (property == ananas::Utils::Identifiers::ModulesParamID) {
            updateModuleLists(treeWhosePropertyHasChanged[property]);
        }
    }

    void WFSInterfaceComponent::parameterChanged(const juce::String &parameterID, const float newValue)
    {
        if (parameterID == Params::ShowModuleSelectors.id) {
            const auto show{newValue > .5f};
            for (auto *m: modules) {
                m->shouldShowModuleSelector(show);
            }
        } else if (parameterID == Params::NumModules.id) {
            modules.clear();
            speakerIcons.clear();
            const auto showModuleSelectors{state.getRawParameterValue(Params::ShowModuleSelectors.id)->load() > .5f};
            const auto numModules{static_cast<int>(newValue)};
            const auto speakerSpacing{state.getRawParameterValue(Params::SpeakerSpacing.id)->load()};
            const auto arrayWidth{newValue * 2.f * speakerSpacing};
            auto x{-arrayWidth / 2.f + speakerSpacing / 2.f};
            // Make module selectors visible
            for (int n{0}; n < numModules; ++n) {
                const auto ss0x{x};
                const auto ss0y{0.f};
                x += speakerSpacing;
                const auto ss1x{x};
                const auto ss1y{0.f};
                x += speakerSpacing;
                const auto m{modules.add(new ModuleComponent(ss0x, ss0y, ss1x, ss1y, persistentTree))};
                addAndMakeVisible(m);
                m->setBroughtToFrontOnMouseClick(true);
                m->shouldShowModuleSelector(showModuleSelectors);
            }
            // Make speaker icons visible
            for (int n{0}; n < 2 * numModules; ++n) {
                const auto s{speakerIcons.add(new SpeakerIconComponent)};
                addAndMakeVisible(s, -1);
            }
            updateModuleLists(persistentTree[ananas::Utils::Identifiers::ModulesParamID]);
        } else if (parameterID == Params::SpeakerSpacing.id) {
            const auto numModules{state.getRawParameterValue(Params::NumModules.id)->load()};
            const auto speakerSpacing{newValue};
            const auto arrayWidth{numModules * 2.f * speakerSpacing};
            auto x{-arrayWidth / 2.f + speakerSpacing / 2.f};
            for (int n{0}; n < static_cast<int>(numModules); ++n) {
                const auto ss0x{x};
                const auto ss0y{0.f};
                x += speakerSpacing;
                const auto ss1x{x};
                const auto ss1y{0.f};
                x += speakerSpacing;
                modules[n]->setSecondarySourceCoordinates(ss0x, ss0y, ss1x, ss1y);
                modules[n]->setCoordinatesForModule();
            }
        }

        resized();
    }

    void WFSInterfaceComponent::expandModuleList(const int moduleID)
    {
        for (auto *m: modules) {
            m->collapseModuleList();
        }
        modules[moduleID]->expandModuleList();
    }
} // ananas::WFS
