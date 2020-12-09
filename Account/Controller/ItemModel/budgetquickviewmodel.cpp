#include "budgetquickviewmodel.h"

int BudgetQuickviewModel::rowCount(const QModelIndex &) const
{
	return m_list.uniqueKeys().size();
}

QHash<int, QByteArray> BudgetQuickviewModel::roleNames() const
{
	static QHash<int, QByteArray> ret = {{(int)BudgetQuickRole::BeginRole, "begin"},
																			 {(int)BudgetQuickRole::EndRole, "end"},
																			 {(int)BudgetQuickRole::NameRole, "name"},
																			 {(int)BudgetQuickRole::CurrentRole, "current"},
																			 {(int)BudgetQuickRole::TargetRole, "target"}};

	return ret;
}

QVariant BudgetQuickviewModel::data(const QModelIndex &index, int role) const
{
	if(index.isValid() && index.row() < rowCount())
	{
		auto row = index.row();
		auto id = m_list.uniqueKeys()[row];

		switch(BudgetQuickRole(role))
		{
			case BudgetQuickRole::NameRole:
				return m_budget[id].category().name();
			case BudgetQuickRole::BeginRole:
				return m_list.values(id).first().begin();
			case BudgetQuickRole::EndRole:
				return m_list.values(id).first().end();
			case BudgetQuickRole::TargetRole:
				return m_list.values(id).first().target();
			case BudgetQuickRole::CurrentRole:
				return m_list.values(id).first().current();
		}
	}

	return QVariant();
}

void BudgetQuickviewModel::setBudget(QMap<QUuid, Budget> list)
{
	m_budget.clear();
	m_budget = list;
}

void BudgetQuickviewModel::reload()
{
	beginRemoveRows(QModelIndex(), 0, rowCount());
	removeRows(0, rowCount());
	endRemoveRows();

	m_list.clear();
	for(auto it2: m_budget)
	{
		for(auto it3: it2.subs())
		{
			if(m_dateList.isEmpty())
			{
				m_list.insert(it2.id(), it3);
			}
			else
			{
				for(auto it: m_dateList)
				{
					if(it3.in(it))
					{
						m_list.insert(it2.id(), it3);
					}
				}
			}
		}
	}

	beginInsertRows(QModelIndex(), 0, rowCount() - 1);
	insertRows(0, rowCount() - 1);
	endInsertRows();
}

QList<QDate> BudgetQuickviewModel::dateList() const
{
	return m_dateList;
}

void BudgetQuickviewModel::setDateList(QList<QDate> list)
{
	m_dateList = list;
	reload();
	emit dateListChanged();
}

