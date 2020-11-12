#include "budgetmodel.h"

BudgetModel::BudgetModel()
{
    connect(AbstractController::db(), &InterfaceDataSave::s_updateBudget, this, &BudgetModel::onBudgetChanged);
    connect(AbstractController::db(), &InterfaceDataSave::s_updateCategory, this, &BudgetModel::onBudgetChanged);
}

int BudgetModel::rowCount(const QModelIndex &) const
{
    return m_incomes.size() + m_outcomes.size();
}

QVariant BudgetModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() >= rowCount())
        return QVariant();

    int row = index.row();
    auto b = row > m_outcomes.size() ? m_incomes[row - m_outcomes.size()] : m_outcomes[row];
    auto type =  row > m_outcomes.size() ? "income" : "outcome";

    switch (BudgetRole(role)) {
    case BudgetRole::NameRole:
        return b.name();
    case BudgetRole::CategoryRole:
        return b.id();
    case BudgetRole::IdBudgetRole:
        return b.metaData<QString>(type);
    case BudgetRole::HasRole:
        return !QUuid::fromString(b.metaData<QString>(type)).isNull();
    }
}

QHash<int, QByteArray> BudgetModel::roleNames() const
{
    static QHash<int, QByteArray> ret = {{int(BudgetRole::NameRole), "name"},
                                         {int(BudgetRole::CategoryRole),"category"},
                                         {int(BudgetRole::IdBudgetRole),"id"},
                                         {int(BudgetRole::HasRole),"has"}};
    return ret;
}

Qt::ItemFlags BudgetModel::flags(const QModelIndex &) const
{
    return Qt::ItemFlags();
}


void BudgetModel::onBudgetChanged()
{

}
