#include <QtTest>

#include "Controller/budgetcontroller.h"
#include "Controller/inmemorystoragelayer.h"

using namespace Controller;

class BudgetControllerTests : public QObject {
	Q_OBJECT
private slots:
	void isAnAbstractController();
	void saveThenLoad_roundTrips();
	void loadAll_returnsEveryBudget();
	void remove_deletesTheBudget();
};

void BudgetControllerTests::isAnAbstractController() {
	InMemoryStorageLayer layer;
	BudgetController controller(layer);
	QVERIFY(qobject_cast<AbstractController *>(&controller) != nullptr);
}

void BudgetControllerTests::saveThenLoad_roundTrips() {
	InMemoryStorageLayer layer;
	QVERIFY(layer.open());
	BudgetController controller(layer);

	Budget budget;
	budget.setId(QUuid::createUuid());
	budget.setName(QStringLiteral("Groceries"));
	budget.setThreshold(300.0);
	QVERIFY(controller.save(&budget));
	QVERIFY(controller.exists(budget.id()));

	const QScopedPointer<Budget> reloaded(controller.load(budget.id()));
	QVERIFY(reloaded);
	QCOMPARE(reloaded->name(), QStringLiteral("Groceries"));
	QCOMPARE(reloaded->threshold(), 300.0);
}

void BudgetControllerTests::loadAll_returnsEveryBudget() {
	InMemoryStorageLayer layer;
	QVERIFY(layer.open());
	BudgetController controller(layer);

	Budget first;
	first.setId(QUuid::createUuid());
	Budget second;
	second.setId(QUuid::createUuid());
	QVERIFY(controller.save(&first));
	QVERIFY(controller.save(&second));

	const QList<Budget *> all = controller.loadAll();
	QCOMPARE(all.size(), 2);
	qDeleteAll(all);
}

void BudgetControllerTests::remove_deletesTheBudget() {
	InMemoryStorageLayer layer;
	QVERIFY(layer.open());
	BudgetController controller(layer);

	Budget budget;
	budget.setId(QUuid::createUuid());
	QVERIFY(controller.save(&budget));
	QVERIFY(controller.remove(budget.id()));
	QVERIFY(!controller.exists(budget.id()));
}

QTEST_APPLESS_MAIN(BudgetControllerTests)

#include "tst_budgetcontroller.moc"
