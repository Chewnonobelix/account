#include "categorylistmodel.h"

int CategoryListModel::rowCount(const QModelIndex &parent) const
{
    return 0;
}

QVariant CategoryListModel::data(const QModelIndex &index, int role) const
{
    return QVariant();
}

QHash<int, QByteArray> CategoryListModel::roleNames() const
{
    return QHash<int, QByteArray>();
}
