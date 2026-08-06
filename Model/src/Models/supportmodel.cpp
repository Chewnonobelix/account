#include "Model/Models/supportmodel.h"

SupportModel::SupportModel(QObject* parent) : EnumListModel(parent) {
    setEntries({
        {tr("Cash"), static_cast<int>(OpenAccountEnums::Support::Cash)},
        {tr("Cheque"), static_cast<int>(OpenAccountEnums::Support::Cheque)},
        {tr("Card"), static_cast<int>(OpenAccountEnums::Support::Card)},
        {tr("Transfer"), static_cast<int>(OpenAccountEnums::Support::Transfer)},
        {tr("Other"), static_cast<int>(OpenAccountEnums::Support::Other)},
    });
}

OpenAccountEnums::Support SupportModel::valueAt(int row) const {
    return static_cast<OpenAccountEnums::Support>(rawValueAt(row));
}

int SupportModel::indexOfValue(OpenAccountEnums::Support value) const {
    return rawIndexOfValue(static_cast<int>(value));
}
