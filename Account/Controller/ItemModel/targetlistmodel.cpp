#include "targetlistmodel.h"

QString TargetListModel::currentId() const
{
	return m_currentBudgetId.toString();
}

void TargetListModel::setCurrentId(QString id)
{
	beginRemoveRows(QModelIndex(), 0, rowCount());
	removeRows(0, rowCount());
	endRemoveColumns();

	m_currentBudgetId = QUuid::fromString(id);

	beginInsertRows(QModelIndex(), 0, rowCount());
	insertRows(0, rowCount());
	endInsertColumns();

	emit currentIdChanged();
}

int TargetListModel::rowCount(const QModelIndex &) const
{
	return AbstractController::db()->selectBudgets()[currentId()].targets().size();
}

QVariant TargetListModel::data(const QModelIndex &index, int role) const
{
	auto row = index.row();
	Budget b = AbstractController::db()->selectBudgets()[currentId()];

	switch (TargetRole(role)) {
	case TargetRole::DateRole:
		return b.targets().values()[row].date;
	case TargetRole::ValueRole:
		return b.targets().values()[row].target;
	case TargetRole::FrequencyRole:
		return QVariant::fromValue(b.targets().values()[row].frequency);
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
