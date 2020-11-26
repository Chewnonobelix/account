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

int TargetListModel::rowCount(const QModelIndex &) const
{
	return m_list.size();
}

QVariant TargetListModel::data(const QModelIndex &index, int role) const
{
	if(index.isValid() && index.row() < rowCount()) {
		auto row = index.row();

		switch (TargetRole(role)) {
		case TargetRole::DateRole:
			return m_list[row].date;
		case TargetRole::ValueRole:
			return m_list[row].target;
		case TargetRole::FrequencyRole:
			return QVariant::fromValue(m_list[row].frequency);
		}
	}

	return QVariant();
}

QHash<int, QByteArray> TargetListModel::roleNames() const
{
	static QHash<int, QByteArray> ret {{int(TargetRole::DateRole), "date"},
																																			{int(TargetRole::ValueRole), "target"},
																																			{int(TargetRole::FrequencyRole), "frequency"}};

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
