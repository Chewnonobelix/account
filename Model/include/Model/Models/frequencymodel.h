#pragma once

#include "enumlistmodel.h"
#include "enums.h"
#include "model_global.h"
#include <QQmlEngine>

/**
 * @brief Lists OpenAccountEnums::Frequency (Custom, Once, Daily, Weekly,
 * Monthly, Quarterly, Yearly) as an OBComboBox model. The sentinel `LAST`
 * value is not a real frequency and is deliberately left out.
 */
class MODEL_EXPORT FrequencyModel : public EnumListModel {
    Q_OBJECT
    QML_ELEMENT

public:
    explicit FrequencyModel(QObject* parent = nullptr);

    [[nodiscard]] Q_INVOKABLE OpenAccountEnums::Frequency valueAt(int row) const;
    [[nodiscard]] Q_INVOKABLE int indexOfValue(OpenAccountEnums::Frequency value) const;
};
