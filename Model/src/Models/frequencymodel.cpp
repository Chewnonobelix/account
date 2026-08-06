#include "Model/Models/frequencymodel.h"

FrequencyModel::FrequencyModel(QObject* parent) : EnumListModel(parent) {
    setEntries({
        {tr("Custom"), static_cast<int>(OpenAccountEnums::Frequency::Custom)},
        {tr("Once"), static_cast<int>(OpenAccountEnums::Frequency::Once)},
        {tr("Daily"), static_cast<int>(OpenAccountEnums::Frequency::Daily)},
        {tr("Weekly"), static_cast<int>(OpenAccountEnums::Frequency::Weekly)},
        {tr("Monthly"), static_cast<int>(OpenAccountEnums::Frequency::Monthly)},
        {tr("Quarterly"), static_cast<int>(OpenAccountEnums::Frequency::Quarterly)},
        {tr("Yearly"), static_cast<int>(OpenAccountEnums::Frequency::Yearly)},
    });
}

OpenAccountEnums::Frequency FrequencyModel::valueAt(int row) const {
    return static_cast<OpenAccountEnums::Frequency>(rawValueAt(row));
}

int FrequencyModel::indexOfValue(OpenAccountEnums::Frequency value) const {
    return rawIndexOfValue(static_cast<int>(value));
}
