#include "targetlistmodel.h"

QString TargetListModel::currentId() const
{
	return m_currentBudgetId.toString();
}

void TargetListModel::setCurrentId(QString id)
{
	beginRemoveRows(QModelIndex(), 0, rowCount());
	removeRows(0, rowCount());
	endRemoveRows();

	m_currentBudgetId = QUuid::fromString(id);

	m_list = AbstractController::db()->selectBudgets()[m_currentBudgetId].targets().values();

	beginInsertRows(QModelIndex(), 0, rowCount() - 1);
	insertRows(0, rowCount() - 1);
	endInsertRows();

	emit currentIdChanged();
}

QVariant TargetListModel::budget() const
{
	return m_budget;
}

void TargetListModel::setBudget(QVariant budget)
{
	m_budget = budget;
	emit budgetChanged();
}

int TargetListModel::rowCount(const QModelIndex &) const
{
	return m_list.size();
}

QVariant TargetListModel::allSubs() const
{
	auto subs = m_budget.value<Budget>().subs();
	QVariantList ret;
	for(auto it: subs) {
		ret<<QVariant::fromValue(it);
	}
	return ret;
}

QVariant TargetListModel::data(const QModelIndex &index, int role) const
{
	if(index.isValid() && index.row() < rowCount()) {
		auto row = index.row();

		static int old = -1;
		static bool selected = false;

		selected = old != row;
		old = row == old ? -1 : row;

		switch (TargetRole(role)) {
		case TargetRole::DateRole:
			return m_list[row].date;
		case TargetRole::ValueRole:
			return m_list[row].target;
		case TargetRole::FrequencyRole:
			return QVariant::fromValue(m_list[row].frequency);
		case TargetRole::SubRole: {
			auto subs = m_budget.value<Budget>().subs();
			QVariantList ret;
			if(selected)
			{
				for(auto it: subs) {
					if(it.reference() == m_list[row].date)
						ret<<QVariant::fromValue(it);
				}
				return ret;
			}
			else
			{
				return allSubs();
			}
		}
		}
	}

	return QVariant();
}

QHash<int, QByteArray> TargetListModel::roleNames() const
{
	static QHash<int, QByteArray> ret {{int(TargetRole::DateRole), "date"},
																																			{int(TargetRole::ValueRole), "target"},
																																			{int(TargetRole::FrequencyRole), "frequency"},
																																			{int(TargetRole::SubRole), "subs"}};

	return ret;
}

Qt::ItemFlags TargetListModel::flags(const QModelIndex &) const
{
	return Qt::ItemFlags();
}

bool TargetListModel::isValid() const
{
	return !m_currentBudgetId.isNull();
}

QString TargetListModel::name() const
{
	return m_budget.value<Budget>().category().name();
}
