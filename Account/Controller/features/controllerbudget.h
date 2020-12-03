#ifndef CONTROLLERBUDGET_H
#define CONTROLLERBUDGET_H

#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlContext>
#include "../core/abstractcontroller.h"
#include "Model/budget.h"
#include "featurebuilder.h"
#include "filler.h"

class ControllerBudget: public AbstractController, public FeatureBuilder
{
	Q_OBJECT

	private:
	QMap<QString, QObject*> m_views;
	QMap<QUuid, Budget> m_budgets;
	QDate m_currentDate;
	QString m_selected;
	Filler<QUuid, Budget> m_filler;

	public:
	ControllerBudget() = default;
	ControllerBudget(const ControllerBudget &);

	~ControllerBudget();
	bool removeFrom(QUuid);

	int exec();
	void openManager();
	void reload();

	QSharedPointer<FeatureBuilder> build(QQmlApplicationEngine *, QObject *);
	QString displayText() const;
	QString baseText() const;
	void checker() {}

	Q_INVOKABLE QVariant get(QString) const;

	public slots:
	void calDateChange(QVariantList);
	void show(QDate);
	void closeManager();
	void addTarget(QString);
	void removeTarget(QString, QString);
	void editReference(QVariant);
	void showTarget(QString, QString, bool);

	void addBudget(QString, int type);
	void removeBudget(QString);
	void editBudget(QString);
	void getTarget(QString);

	void updateEntry(QUuid);
	void changeEntry(QString, QUuid);

	signals:
	void selectCat(QString);
	void blocked(bool);
	void dateChanged(QVariant);
	void clearDate();
	void catChanged(QString);
	void addCat(QVariant);
	void clearCat();
	void addTarget2(QVariant);
	void clearTarget();
	void addSub(QVariant);
	void clearSub();
	void close();
};

Q_DECLARE_METATYPE(ControllerBudget)
#endif // CONTROLLERBUDGET_H
