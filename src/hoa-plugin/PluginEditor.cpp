#include "PluginEditor.h"
#include <AnanasUtils.h>
#include "HOAUtils.h"

PluginEditor::PluginEditor(PluginProcessor &p)
    : AudioProcessorEditor(&p),
      tooltipWindow(this, ananas::UI::Timing::TooltipDelayTimeMs),
      networkOverview(
          getProcessor().getDynamicTree(),
          getProcessor().getPersistentTree()
      ),
      hoaInterface(
          ananas::HOA::Constants::NumChannels,
          getProcessor().getParamState(),
          getProcessor().getPersistentTree()
      )
{
    juce::Desktop::getInstance().setDefaultLookAndFeel(&lookAndFeel);

    setLookAndFeel(&lookAndFeel);

    addAndMakeVisible(tabbedComponent);
    tabbedComponent.addTab(
        ananas::HOA::Strings::HoaTabName,
        juce::Colours::whitesmoke,
        &hoaInterface,
        false
    );
    tabbedComponent.addTab(
        ananas::HOA::Strings::NetworkTabName,
        juce::Colours::whitesmoke,
        &networkOverview,
        false
    );
    lookAndFeel.setNumberOfTabs(tabbedComponent.getNumTabs());

    setSize(ananas::UI::Dimensions::UiWidth, ananas::UI::Dimensions::UiHeight);

    getProcessor().getPersistentTree().addListener(this);
    getProcessor().getDynamicTree().addListener(this);

#if SHOW_NO_NETWORK_OVERLAY
    getProcessor().getServer().addChangeListener(&hoaInterface);
    getProcessor().getServer().addChangeListener(&networkOverview);
    getProcessor().getServer().sendChangeMessage();
#endif

    commandManager.setFirstCommandTarget(this);
    commandManager.registerAllCommandsForTarget(this);
    addKeyListener(commandManager.getKeyMappings());
    setWantsKeyboardFocus(true);
}

PluginEditor::~PluginEditor()
{
    juce::Desktop::getInstance().setDefaultLookAndFeel(nullptr);
    setLookAndFeel(nullptr);

    getProcessor().getPersistentTree().removeListener(this);
    getProcessor().getDynamicTree().removeListener(this);

#if SHOW_NO_NETWORK_OVERLAY
    getProcessor().getServer().removeChangeListener(&hoaInterface);
    getProcessor().getServer().removeChangeListener(&networkOverview);
#endif
}

void PluginEditor::paint(juce::Graphics &g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void PluginEditor::resized()
{
    lookAndFeel.setTotalWidth(getWidth());
    tabbedComponent.setBounds(getLocalBounds());
}

void PluginEditor::valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyHasChanged, const juce::Identifier &property)
{
    if (property == ananas::Utils::Identifiers::SwitchesParamID) {
        getProcessor().getServer().getSwitches()->handleEdit(treeWhosePropertyHasChanged[property]);
    } else if (property == ananas::Utils::Identifiers::ClientsShouldRebootParamID) {
        getProcessor().getServer().getClientList()->setShouldReboot(treeWhosePropertyHasChanged[property]);
        treeWhosePropertyHasChanged.setPropertyExcludingListener(this, property, false, nullptr);
    }
}

juce::ApplicationCommandTarget *PluginEditor::getNextCommandTarget()
{
    return nullptr;
}

void PluginEditor::getAllCommands(juce::Array<int> &commands)
{
    commands.addArray({
        ananas::HOA::SwitchToHoaTab,
        ananas::HOA::SwitchToNetworkTab,
        ananas::HOA::ToggleModuleSelectorDisplay
    });

    for (size_t m{0}; m < 10; m++) {
        commands.add(ananas::HOA::RevealModuleSelector + static_cast<int>(m));
    }
}

void PluginEditor::getCommandInfo(const juce::CommandID commandID, juce::ApplicationCommandInfo &result)
{
    switch (commandID) {
        case ananas::HOA::SwitchToHoaTab:
            result.setInfo(
                ananas::HOA::Strings::SwitchToHoaTabShortName,
                ananas::HOA::Strings::SwitchToHoaTabDescription,
                ananas::HOA::Strings::TabsCommandCategoryName,
                0);
            result.addDefaultKeypress(ananas::HOA::UI::Shortcuts::SwitchToHoaTabKeycode, juce::ModifierKeys::noModifiers);
            break;

        case ananas::HOA::SwitchToNetworkTab:
            result.setInfo(
                ananas::HOA::Strings::SwitchToNetworkTabShortName,
                ananas::HOA::Strings::SwitchToNetworkTabDescription,
                ananas::HOA::Strings::TabsCommandCategoryName,
                0);
            result.addDefaultKeypress(ananas::HOA::UI::Shortcuts::SwitchToNetworkTabKeycode, juce::ModifierKeys::noModifiers);
            break;

        case ananas::HOA::ToggleModuleSelectorDisplay:
            result.setInfo(
                ananas::HOA::Strings::ToggleModuleSelectorsShortName,
                ananas::HOA::Strings::ToggleModuleSelectorsDescription,
                ananas::HOA::Strings::OptionsCommandCategoryName,
                hoaInterface.isVisible() ? 0 : juce::ApplicationCommandInfo::CommandFlags::isDisabled);
            result.addDefaultKeypress(ananas::HOA::UI::Shortcuts::ToggleModuleSelectorsKeycode, juce::ModifierKeys::noModifiers);
            break;

        default:
            const auto moduleID{commandID - ananas::HOA::RevealModuleSelector - 1};
            const auto moduleSelectorsAreVisible{
                getProcessor().getParamState().getRawParameterValue(ananas::HOA::Params::ShowModuleSelectors.id)->load() > .5f
            };
            result.setInfo(
                ananas::HOA::Strings::getRevealModuleSelectorShortName(moduleID),
                ananas::HOA::Strings::getRevealModuleSelectorDescription(moduleID),
                ananas::HOA::Strings::ModulesCommandCategoryName,
                hoaInterface.isVisible() && moduleSelectorsAreVisible ? 0 : juce::ApplicationCommandInfo::CommandFlags::isDisabled);
            result.addDefaultKeypress(
                ananas::HOA::UI::Shortcuts::RevealModuleSelectorListKeycode + commandID - ananas::HOA::RevealModuleSelector,
                juce::ModifierKeys::noModifiers
            );
            break;
    }
}

bool PluginEditor::perform(const InvocationInfo &info)
{
    switch (info.commandID) {
        case ananas::HOA::SwitchToHoaTab:
            tabbedComponent.setCurrentTabIndex(0);
            return true;

        case ananas::HOA::SwitchToNetworkTab:
            tabbedComponent.setCurrentTabIndex(1);
            return true;

        case ananas::HOA::ToggleModuleSelectorDisplay: {
            const auto showModuleSelectors{getProcessor().getParamState().getParameter(ananas::HOA::Params::ShowModuleSelectors.id)};
            showModuleSelectors->setValueNotifyingHost(fabsf(showModuleSelectors->getValue() - 1.f));
            return true;
        }

        default:
            hoaInterface.expandModuleList(info.commandID - ananas::HOA::RevealModuleSelector);
            return true;
    }
}

PluginProcessor &PluginEditor::getProcessor()
{
    return dynamic_cast<PluginProcessor &>(processor);
}

const PluginProcessor &PluginEditor::getProcessor() const
{
    return dynamic_cast<const PluginProcessor &>(processor);
}
