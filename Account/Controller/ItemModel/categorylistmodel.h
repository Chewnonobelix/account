#ifndef CATEGORYLISTMODEL_H
#define CATEGORYLISTMODEL_H

#include "../core/abstractcontroller.h"
#include "Model/accountglobal.h"
#include "Model/category.h"
#include <QAbstractListModel>
#include <QtCore/qglobal.h>

class CategoryListModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(Account::TypeEnum currentType READ currentType WRITE setCurrentType NOTIFY
                   currentTypeChanged)

private:
    Account::TypeEnum m_currentType = Account::TypeEnum::Outcome;
    QMap<Account::TypeEnum, QMap<QUuid, Category>> m_categories;
    int m_currentIndex = 0;
    InterfaceDataSave *m_db;

    void reset();
    void init();

public:
    enum class CategoryRole { DisplayRole = Qt::UserRole + 1, TypeRole, IndexRole };
    Q_ENUM(CategoryRole)

    CategoryListModel();
    CategoryListModel(const CategoryListModel &) = default;
    ~CategoryListModel() = default;

    Account::TypeEnum currentType() const;
    void setCurrentType(Account::TypeEnum);

public slots:
    void onUpdateCategory();

signals:
    void currentTypeChanged();

public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    Qt::ItemFlags flags(const QModelIndex &) const override;
};

#endif // CATEGORYLISTMODEL_H
