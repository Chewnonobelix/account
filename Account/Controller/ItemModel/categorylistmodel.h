#ifndef CATEGORYLISTMODEL_H
#define CATEGORYLISTMODEL_H

#include "Model/accountglobal.h"
#include "Model/category.h"
#include <QAbstractListModel>
#include <QtCore/qglobal.h>

class CategoryListModel : public QAbstractItemModel
{
    Q_OBJECT

private:
    Account::TypeEnum m_currentType;
    QMap<Account::TypeEnum, QMap<QUuid, Category>> m_categories;
    int m_currentIndex;

public:
    enum class CategoryRole { DisplayRole = Qt::UserRole + 1, TypeRole };

    CategoryListModel() = default;
    CategoryListModel(const CategoryListModel &) = default;
    ~CategoryListModel() = default;

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;
};

#endif // CATEGORYLISTMODEL_H
