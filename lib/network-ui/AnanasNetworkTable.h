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
            textColourId = 0x2001800,
            okColourId = 0x2001801,
            warningColourId = 0x2001802,
        };

        [[nodiscard]] juce::Justification::Flags getJustification(int columnId) const;

    protected:
        void addColumn(const TableColumns::ColumnHeader &h);

        juce::TableListBox table;

    private:
        juce::HashMap<int, juce::Justification::Flags> justifications;
    };
}

#endif //ANANASNETWORKTABLE_H
