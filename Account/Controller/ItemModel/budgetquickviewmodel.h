#ifndef BUDGETQUICKVIEWMODEL_H
#define BUDGETQUICKVIEWMODEL_H

#include <QAbstractListModel>
#include "Model/subbudget.h"
#include "Model/budget.h"

class BudgetQuickviewModel: public QAbstractListModel
{
	Q_OBJECT

	Q_PROPERTY(QList<QDate> dateList READ dateList WRITE setDateList NOTIFY dateListChanged)

	private:
	enum class BudgetQuickRole {NameRole = Qt::UserRole + 1,
																														BeginRole, EndRole,
																														TargetRole, CurrentRole};

	QList<QDate> m_dateList;
	QMultiMap<QUuid, SubBudget> m_list;
	QMap<QUuid, Budget> m_budget;

	void reload();

	public:
	BudgetQuickviewModel() = default;
	~BudgetQuickviewModel() override = default;

	void setBudget(QMap<QUuid, Budget>);

	QList<QDate> dateList() const;
	void setDateList(QList<QDate>);

	signals:
	void dateListChanged();

	public:
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QHash<int, QByteArray> roleNames() const override;
	QVariant data(const QModelIndex &index, int role) const override;
};

#endif // BUDGETQUICKVIEWMODEL_H
