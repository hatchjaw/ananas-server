#ifndef ANANASNETWORKTABLE_H
#define ANANASNETWORKTABLE_H

#include <juce_gui_basics/juce_gui_basics.h>
#include "AnanasUIUtils.h"

namespace ananas::UI
{
    class AnanasNetworkTable : public juce::Component,
                               public juce::TableListBoxModel
    {
    public:
        enum ColourIds
        {
            okColour = 0x2001800,
            warningColour = 0x2001801,
        };
    protected:
        void addColumn(const TableColumns::ColumnHeader &h);

        [[nodiscard]] juce::Justification::Flags getJustification(int columnId) const;

        juce::TableListBox table;

    private:
        juce::HashMap<int, juce::Justification::Flags> justifications;
    };
}

#endif //ANANASNETWORKTABLE_H
