#ifndef TARGETLISTMODEL_H
#define TARGETLISTMODEL_H

#include <QAbstractListModel>
#include "../core/abstractcontroller.h"

class TargetListModel: public QAbstractListModel
{
	Q_OBJECT
	Q_PROPERTY(QString currentId READ currentId WRITE setCurrentId NOTIFY currentIdChanged)
	Q_PROPERTY(bool isValid READ isValid CONSTANT)

private:
				enum class TargetRole {DateRole = Qt::UserRole + 1, ValueRole, FrequencyRole};

				QUuid m_currentBudgetId;

public:
	TargetListModel() = default;
	~TargetListModel() = default;

	QString currentId() const;
	void setCurrentId(QString);
	bool isValid() const;

	signals:
						void currentIdChanged();
public:
	Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	Q_INVOKABLE QVariant data(const QModelIndex &index, int role) const override;
	Q_INVOKABLE QHash<int, QByteArray> roleNames() const override;
	Q_INVOKABLE Qt::ItemFlags flags(const QModelIndex &) const override;

};

#endif // TARGETLISTMODEL_H
