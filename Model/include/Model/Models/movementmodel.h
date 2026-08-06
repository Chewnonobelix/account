#pragma once

#include "enumlistmodel.h"
#include "enums.h"
#include "model_global.h"
#include <QQmlEngine>

/**
 * @brief Lists OpenAccountEnums::Movement (Both, Credit, Debit) as an
 * OBComboBox model, e.g. for a Category's or Debt's direction.
 */
class MODEL_EXPORT MovementModel : public EnumListModel {
    Q_OBJECT
    QML_ELEMENT

public:
    explicit MovementModel(QObject* parent = nullptr);

    [[nodiscard]] Q_INVOKABLE OpenAccountEnums::Movement valueAt(int row) const;
    [[nodiscard]] Q_INVOKABLE int indexOfValue(OpenAccountEnums::Movement value) const;
};
