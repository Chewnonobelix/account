#ifndef TARGETLISTMODEL_H
#define TARGETLISTMODEL_H

#include <QAbstractListModel>
#include "../core/abstractcontroller.h"

class TargetListModel: public QAbstractListModel
{
	Q_OBJECT
	Q_PROPERTY(QString currentId READ currentId WRITE setCurrentId NOTIFY currentIdChanged)
	Q_PROPERTY(bool isValid READ isValid CONSTANT)
	Q_PROPERTY(QString name READ name CONSTANT)
	Q_PROPERTY(QVariant budget READ budget WRITE setBudget NOTIFY budgetChanged)

	private:
	QUuid m_currentBudgetId;
	QList<Target> m_list;
	QVariant m_budget;

	public:
	enum class TargetRole {DateRole = Qt::UserRole + 1, ValueRole, FrequencyRole, SubRole};
	TargetListModel() = default;
	~TargetListModel() = default;

	QString currentId() const;
	void setCurrentId(QString);

	QVariant budget() const;
	void setBudget(QVariant);

	QString name() const;
	bool isValid() const;

	Q_INVOKABLE QVariant allSubs() const;

	signals:

	void currentIdChanged();
	void budgetChanged();

	public:
	Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	Q_INVOKABLE QVariant data(const QModelIndex &index, int role) const override;
	Q_INVOKABLE QHash<int, QByteArray> roleNames() const override;
	Q_INVOKABLE Qt::ItemFlags flags(const QModelIndex &) const override;

};

#endif // TARGETLISTMODEL_H
