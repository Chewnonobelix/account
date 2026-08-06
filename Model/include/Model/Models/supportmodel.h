#pragma once

#include "enumlistmodel.h"
#include "enums.h"
#include "model_global.h"
#include <QQmlEngine>

/**
 * @brief Lists OpenAccountEnums::Support (Cash, Cheque, Card, Transfer,
 * Other) as an OBComboBox model, e.g. for a Transaction's payment method.
 */
class MODEL_EXPORT SupportModel : public EnumListModel {
    Q_OBJECT
    QML_ELEMENT

public:
    explicit SupportModel(QObject* parent = nullptr);

    [[nodiscard]] Q_INVOKABLE OpenAccountEnums::Support valueAt(int row) const;
    [[nodiscard]] Q_INVOKABLE int indexOfValue(OpenAccountEnums::Support value) const;
};
