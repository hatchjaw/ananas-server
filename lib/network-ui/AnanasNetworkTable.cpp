#include "AnanasNetworkTable.h"

namespace ananas::UI {
    void AnanasNetworkTable::addColumn(const TableColumns::ColumnHeader &h)
    {
        table.getHeader().addColumn(h.label, h.id, h.width, h.minWidth, h.maxWidth, h.flags);
        justifications.set(h.id, h.justification);
    }

    juce::Justification::Flags AnanasNetworkTable::getJustification(const int columnId) const
    {
        return justifications[columnId];
    }
}
