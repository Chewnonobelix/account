#include <QtTest>

#include "Controller/accountcontroller.h"
#include "Controller/inmemorystoragelayer.h"

using namespace Controller;

class AccountControllerTests : public QObject {
	Q_OBJECT
private slots:
	void isAnAbstractController();
	void saveThenLoad_roundTrips();
	void loadAll_returnsEveryAccount();
	void remove_deletesTheAccount();
};

void AccountControllerTests::isAnAbstractController() {
	InMemoryStorageLayer layer;
	AccountController controller(layer);
	QVERIFY(qobject_cast<AbstractController *>(&controller) != nullptr);
}

void AccountControllerTests::saveThenLoad_roundTrips() {
	InMemoryStorageLayer layer;
	QVERIFY(layer.open());
	AccountController controller(layer);

	Account account;
	account.setId(QUuid::createUuid());
	account.setBank(QStringLiteral("BNP"));
	QVERIFY(controller.save(&account));
	QVERIFY(controller.exists(account.id()));

	const QScopedPointer<Account> reloaded(controller.load(account.id()));
	QVERIFY(reloaded);
	QCOMPARE(reloaded->bank(), QStringLiteral("BNP"));
}

void AccountControllerTests::loadAll_returnsEveryAccount() {
	InMemoryStorageLayer layer;
	QVERIFY(layer.open());
	AccountController controller(layer);

	Account first;
	first.setId(QUuid::createUuid());
	Account second;
	second.setId(QUuid::createUuid());
	QVERIFY(controller.save(&first));
	QVERIFY(controller.save(&second));

	const QList<Account *> all = controller.loadAll();
	QCOMPARE(all.size(), 2);
	qDeleteAll(all);
}

void AccountControllerTests::remove_deletesTheAccount() {
	InMemoryStorageLayer layer;
	QVERIFY(layer.open());
	AccountController controller(layer);

	Account account;
	account.setId(QUuid::createUuid());
	QVERIFY(controller.save(&account));
	QVERIFY(controller.remove(account.id()));
	QVERIFY(!controller.exists(account.id()));
}

QTEST_APPLESS_MAIN(AccountControllerTests)

#include "tst_accountcontroller.moc"
