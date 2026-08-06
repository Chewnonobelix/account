#include "Model/Models/movementmodel.h"

MovementModel::MovementModel(QObject* parent) : EnumListModel(parent) {
    setEntries({
        {tr("Both"), static_cast<int>(OpenAccountEnums::Movement::Both)},
        {tr("Credit"), static_cast<int>(OpenAccountEnums::Movement::Credit)},
        {tr("Debit"), static_cast<int>(OpenAccountEnums::Movement::Debit)},
    });
}

OpenAccountEnums::Movement MovementModel::valueAt(int row) const {
    return static_cast<OpenAccountEnums::Movement>(rawValueAt(row));
}

int MovementModel::indexOfValue(OpenAccountEnums::Movement value) const {
    return rawIndexOfValue(static_cast<int>(value));
}
