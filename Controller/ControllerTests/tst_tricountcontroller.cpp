#include <QtTest>

#include "Controller/inmemorystoragelayer.h"
#include "Controller/tricountcontroller.h"

using namespace Controller;

class TricountControllerTests : public QObject {
	Q_OBJECT
private slots:
	void isAnAbstractController();
	void saveThenLoad_roundTrips();
	void loadAll_returnsEveryTricount();
	void remove_deletesTheTricount();
};

void TricountControllerTests::isAnAbstractController() {
	InMemoryStorageLayer layer;
	TricountController controller(layer);
	QVERIFY(qobject_cast<AbstractController *>(&controller) != nullptr);
}

void TricountControllerTests::saveThenLoad_roundTrips() {
	InMemoryStorageLayer layer;
	QVERIFY(layer.open());
	TricountController controller(layer);

	Tricount tricount;
	tricount.setId(QUuid::createUuid());
	tricount.setName(QStringLiteral("Ski trip"));
	tricount.setCurrency(QStringLiteral("EUR"));
	QVERIFY(controller.save(&tricount));
	QVERIFY(controller.exists(tricount.id()));

	const QScopedPointer<Tricount> reloaded(controller.load(tricount.id()));
	QVERIFY(reloaded);
	QCOMPARE(reloaded->name(), QStringLiteral("Ski trip"));
	QCOMPARE(reloaded->currency(), QStringLiteral("EUR"));
}

void TricountControllerTests::loadAll_returnsEveryTricount() {
	InMemoryStorageLayer layer;
	QVERIFY(layer.open());
	TricountController controller(layer);

	Tricount first;
	first.setId(QUuid::createUuid());
	Tricount second;
	second.setId(QUuid::createUuid());
	QVERIFY(controller.save(&first));
	QVERIFY(controller.save(&second));

	const QList<Tricount *> all = controller.loadAll();
	QCOMPARE(all.size(), 2);
	qDeleteAll(all);
}

void TricountControllerTests::remove_deletesTheTricount() {
	InMemoryStorageLayer layer;
	QVERIFY(layer.open());
	TricountController controller(layer);

	Tricount tricount;
	tricount.setId(QUuid::createUuid());
	QVERIFY(controller.save(&tricount));
	QVERIFY(controller.remove(tricount.id()));
	QVERIFY(!controller.exists(tricount.id()));
}

QTEST_APPLESS_MAIN(TricountControllerTests)

#include "tst_tricountcontroller.moc"
