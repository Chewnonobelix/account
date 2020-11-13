#include "budgetmodel.h"

BudgetModel::BudgetModel()
{
    connect(AbstractController::db(), &InterfaceDataSave::s_updateBudget, this, &BudgetModel::onBudgetChanged);
    connect(AbstractController::db(), &InterfaceDataSave::s_updateCategory, this, &BudgetModel::onBudgetChanged);
}

int BudgetModel::rowCount(const QModelIndex &) const
{
    return m_list.size();
}

QVariant BudgetModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() >= rowCount())
        return QVariant();

    int row = index.row();
    auto c =  m_list[row];

    auto t = QMetaEnum::fromType<Account::TypeEnum>().valueToKeys((int)type());
    switch (BudgetRole(role)) {
    case BudgetRole::NameRole:
        return c.name();
    case BudgetRole::CategoryRole:
        return c.id();
    case BudgetRole::IdBudgetRole:
        return c.metaData<QString>(t);
    case BudgetRole::HasRole:
        return !QUuid::fromString(c.metaData<QString>(t)).isNull();
    }
}

QHash<int, QByteArray> BudgetModel::roleNames() const
{
    static QHash<int, QByteArray> ret = {{int(BudgetRole::NameRole), "name"},
                                         {int(BudgetRole::CategoryRole),"category"},
                                         {int(BudgetRole::IdBudgetRole),"idBudget"},
                                         {int(BudgetRole::HasRole),"has"}};
    return ret;
}

Qt::ItemFlags BudgetModel::flags(const QModelIndex &) const
{
    return Qt::ItemFlags();
}


void BudgetModel::onBudgetChanged()
{
    clear();
    m_list = AbstractController::db()->selectCategory()[type()].values();
    prepareRow();
}

Account::TypeEnum BudgetModel::type() const
{
    return m_type;
}

void BudgetModel::setType(Account::TypeEnum type)
{
    m_type = type;
    emit typeChanged();
    onBudgetChanged();
}

void BudgetModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, rowCount());
    removeRows(0, rowCount());
    endRemoveRows();
}

void BudgetModel::prepareRow()
{
    beginInsertRows(QModelIndex(), 0, rowCount() - 1);
    insertRows(0, rowCount()-1);
    endInsertRows();
}
