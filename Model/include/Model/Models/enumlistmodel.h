#pragma once

#include "model_global.h"
#include <QAbstractListModel>
#include <QString>
#include <QVector>

/**
 * @brief Base for the read-only "name -> enum value" list models exposed to
 * QML (MovementModel, SupportModel, FrequencyModel, ...), so OBComboBox can
 * list an enum's values without every caller re-declaring parallel
 * names/values arrays in QML.
 *
 * Not itself registered to QML: it has no QML_ELEMENT and no public
 * enum-typed accessors, since the enum type is only known to the subclass.
 */
class MODEL_EXPORT EnumListModel : public QAbstractListModel {
    Q_OBJECT

public:
    explicit EnumListModel(QObject* parent = nullptr);

    [[nodiscard]] int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    // Display text for a row, e.g. to render a read-only label for the
    // enumerator currently held by a bound model (indexOfValue(value) then
    // textAt(row)) without needing QAbstractItemModel::data() from QML.
    [[nodiscard]] Q_INVOKABLE QString textAt(int row) const;

protected:
    struct Entry {
        QString text;
        int value;
    };

    void setEntries(QVector<Entry> entries);
    [[nodiscard]] int rawValueAt(int row) const;
    [[nodiscard]] int rawIndexOfValue(int value) const;

private:
    QVector<Entry> m_entries;
};
