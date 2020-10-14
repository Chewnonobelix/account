#ifndef MAINMODEL_H
#define MAINMODEL_H

#include<QAbstractItemModel>
#include "Model/entry.h"
#include "Model/total.h"
#include <QList>

class MainModel: public QAbstractItemModel
{
public:
    MainModel() = default;

public: //Model methods
    Qt::ItemFlags flags(QModelIndex const & = QModelIndex()) const;
    QVariant data(QModelIndex const &, int) const;
    QVariant headerData(int, Qt::Orientation, int) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

};

Q_DECLARE_METATYPE(MainModel)

#endif // MAINMODEL_H
