#ifndef CATEGORYLISTMODEL_H
#define CATEGORYLISTMODEL_H

#include <QAbstractListModel>
#include <QtCore/qglobal.h>

class CategoryListModel : public QAbstractItemModel
{
    Q_OBJECT
private:
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
