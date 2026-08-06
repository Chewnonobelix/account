#include <QtTest>

#include "Controller/inmemorystoragelayer.h"
#include "Controller/transactioncontroller.h"

using namespace Controller;

class TransactionControllerTests : public QObject {
	Q_OBJECT
private slots:
	void isAnAbstractController();
	void saveThenLoad_roundTrips();
	void loadAll_returnsEveryTransaction();
	void remove_deletesTheTransaction();
};

void TransactionControllerTests::isAnAbstractController() {
	InMemoryStorageLayer layer;
	TransactionController controller(layer);
	QVERIFY(qobject_cast<AbstractController *>(&controller) != nullptr);
}

void TransactionControllerTests::saveThenLoad_roundTrips() {
	InMemoryStorageLayer layer;
	QVERIFY(layer.open());
	TransactionController controller(layer);

	const TransactionPtr transaction = TransactionPtr::create();
	transaction->setId(QUuid::createUuid());
	transaction->setDescription(QStringLiteral("Groceries"));
	transaction->setValue(42.5);
	QVERIFY(controller.save(transaction));
	QVERIFY(controller.exists(transaction->id()));

	const TransactionPtr reloaded = controller.load(transaction->id());
	QVERIFY(reloaded);
	QCOMPARE(reloaded->description(), QStringLiteral("Groceries"));
	QCOMPARE(reloaded->value(), 42.5);
}

void TransactionControllerTests::loadAll_returnsEveryTransaction() {
	InMemoryStorageLayer layer;
	QVERIFY(layer.open());
	TransactionController controller(layer);

	const TransactionPtr first = TransactionPtr::create();
	first->setId(QUuid::createUuid());
	const TransactionPtr second = TransactionPtr::create();
	second->setId(QUuid::createUuid());
	QVERIFY(controller.save(first));
	QVERIFY(controller.save(second));

	QCOMPARE(controller.loadAll().size(), 2);
}

void TransactionControllerTests::remove_deletesTheTransaction() {
	InMemoryStorageLayer layer;
	QVERIFY(layer.open());
	TransactionController controller(layer);

	const TransactionPtr transaction = TransactionPtr::create();
	transaction->setId(QUuid::createUuid());
	QVERIFY(controller.save(transaction));
	QVERIFY(controller.remove(transaction->id()));
	QVERIFY(!controller.exists(transaction->id()));
}

QTEST_APPLESS_MAIN(TransactionControllerTests)

#include "tst_transactioncontroller.moc"
