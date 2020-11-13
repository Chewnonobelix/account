#ifndef BUDGETMODEL_H
#define BUDGETMODEL_H

#include <QAbstractListModel>
#include "../core/abstractcontroller.h"
#include "../core/abstractcontroller.h"

class BudgetModel: public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(Account::TypeEnum type READ type WRITE setType NOTIFY typeChanged)
private:
    QList<Category> m_list;
    Account::TypeEnum m_type = Account::TypeEnum::Outcome;

public:
    enum class BudgetRole {NameRole = Qt::UserRole + 1,
                            CategoryRole,
                            IdBudgetRole,
                            HasRole
                            };

    BudgetModel();

    Account::TypeEnum type() const;
    void setType(Account::TypeEnum);

    void clear();
    void prepareRow();

public slots:
    void onBudgetChanged();

signals:
    void typeChanged();

public:
    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_INVOKABLE QVariant data(const QModelIndex &index, int role) const override;
    Q_INVOKABLE QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE Qt::ItemFlags flags(const QModelIndex &) const override;
};

#endif // BUDGETMODEL_H
