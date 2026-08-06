#include <QtTest>

#include "Controller/debtcontroller.h"
#include "Controller/inmemorystoragelayer.h"

using namespace Controller;

class DebtControllerTests : public QObject {
	Q_OBJECT
private slots:
	void isAnAbstractController();
	void saveThenLoad_roundTrips();
	void loadAll_returnsEveryDebt();
	void remove_deletesTheDebt();
};

void DebtControllerTests::isAnAbstractController() {
	InMemoryStorageLayer layer;
	DebtController controller(layer);
	QVERIFY(qobject_cast<AbstractController *>(&controller) != nullptr);
}

void DebtControllerTests::saveThenLoad_roundTrips() {
	InMemoryStorageLayer layer;
	QVERIFY(layer.open());
	DebtController controller(layer);

	Debt debt;
	debt.setId(QUuid::createUuid());
	debt.setName(QStringLiteral("Car loan"));
	debt.setPrincipal(10000.0);
	debt.setRemaining(7500.0);
	QVERIFY(controller.save(&debt));
	QVERIFY(controller.exists(debt.id()));

	const QScopedPointer<Debt> reloaded(controller.load(debt.id()));
	QVERIFY(reloaded);
	QCOMPARE(reloaded->name(), QStringLiteral("Car loan"));
	QCOMPARE(reloaded->principal(), 10000.0);
	QCOMPARE(reloaded->remaining(), 7500.0);
}

void DebtControllerTests::loadAll_returnsEveryDebt() {
	InMemoryStorageLayer layer;
	QVERIFY(layer.open());
	DebtController controller(layer);

	Debt first;
	first.setId(QUuid::createUuid());
	Debt second;
	second.setId(QUuid::createUuid());
	QVERIFY(controller.save(&first));
	QVERIFY(controller.save(&second));

	const QList<Debt *> all = controller.loadAll();
	QCOMPARE(all.size(), 2);
	qDeleteAll(all);
}

void DebtControllerTests::remove_deletesTheDebt() {
	InMemoryStorageLayer layer;
	QVERIFY(layer.open());
	DebtController controller(layer);

	Debt debt;
	debt.setId(QUuid::createUuid());
	QVERIFY(controller.save(&debt));
	QVERIFY(controller.remove(debt.id()));
	QVERIFY(!controller.exists(debt.id()));
}

QTEST_APPLESS_MAIN(DebtControllerTests)

#include "tst_debtcontroller.moc"
