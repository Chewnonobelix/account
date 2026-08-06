#include "Model/Models/enumlistmodel.h"

EnumListModel::EnumListModel(QObject* parent) : QAbstractListModel(parent) {}

int EnumListModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid()) return 0;
    return m_entries.size();
}

QVariant EnumListModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= m_entries.size()) return {};
    if (role != Qt::DisplayRole) return {};

    return m_entries.at(index.row()).text;
}

QHash<int, QByteArray> EnumListModel::roleNames() const {
    // A single role only: OBComboBox's delegate reads the row via QML's
    // implicit `modelData`, which Qt only synthesizes for single-role
    // models. Exposing a second ("value") role here would silence
    // `modelData` and break every OBComboBox using this model.
    return {{Qt::DisplayRole, "text"}};
}

void EnumListModel::setEntries(QVector<Entry> entries) {
    beginResetModel();
    m_entries = std::move(entries);
    endResetModel();
}

int EnumListModel::rawValueAt(int row) const {
    if (row < 0 || row >= m_entries.size()) return -1;
    return m_entries.at(row).value;
}

int EnumListModel::rawIndexOfValue(int value) const {
    for (int i = 0; i < m_entries.size(); ++i) {
        if (m_entries.at(i).value == value) return i;
    }
    return -1;
}
