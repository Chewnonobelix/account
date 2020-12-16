#ifndef CONTROLLERBUDGET_H
#define CONTROLLERBUDGET_H

#include "../core/abstractcontroller.h"
#include "Controller/ItemModel/budgetquickviewmodel.h"
#include "Model/budget.h"
#include "featurebuilder.h"
#include "filler.h"
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQuickItem>

class ControllerBudget : public AbstractController, public FeatureBuilder {
	Q_OBJECT

private:
	QMap<QString, QObject *> m_views;
	QMap<QUuid, Budget> m_budgets;
	QDate m_currentDate;
	QString m_selected;
	Filler<QUuid, Budget> m_filler;
	BudgetQuickviewModel m_quickModel;

public:
	ControllerBudget() = default;
	ControllerBudget(const ControllerBudget &);

	~ControllerBudget();
	bool removeFrom(QUuid);

	int exec() override;
	void openManager();
	void reload();

	QSharedPointer<FeatureBuilder> build(QQmlApplicationEngine *,
																			 QObject *) override;
	QString displayText() const override;
	QString baseText() const override;
	void checker() override {}
	void setQuickView(QList<QString>) override;
	Q_INVOKABLE QVariant get(QString) const;

public slots:
	void closeManager();
	void removeTarget(QString, QDate);
	void editReference(QVariant);
	void showTarget(QString, QString, bool);

	void addBudget(QString, int type);
	void removeBudget(QString);

	void updateEntry(QUuid);
	void changeEntry(QString, QUuid);

	void onEndFill();

signals:
	void budgetChanged();
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
