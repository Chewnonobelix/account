#ifndef BUDGETMODEL_H
#define BUDGETMODEL_H

#include <QAbstractListModel>
#include "../core/abstractcontroller.h"
#include "../core/abstractcontroller.h"

class BudgetModel: public QAbstractListModel
{
    Q_OBJECT
private:
    QList<Category> m_incomes;
    QList<Category> m_outcomes;

public:
    enum class BudgetRole {NameRole = Qt::UserRole + 1,
                            CategoryRole,
                            IdBudgetRole,
                            HasRole
                            };

    BudgetModel();

public slots:
    void onBudgetChanged();

public:
    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_INVOKABLE QVariant data(const QModelIndex &index, int role) const override;
    Q_INVOKABLE QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE Qt::ItemFlags flags(const QModelIndex &) const override;
};

#endif // BUDGETMODEL_H
