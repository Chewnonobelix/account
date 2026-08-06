#include <QTemporaryDir>
#include <QtTest>

#include "Controller/entitystorage.h"
#include "Controller/inmemorystoragelayer.h"
#include "Controller/jsonstoragelayer.h"
#include "Model/Business/account.h"

using namespace Controller;

// Exercises the generic EntityStorage<T> template directly (instantiated on
// Account, the simplest entity) so every concrete *Controller only needs a
// thin smoke test of its own wiring.
class EntityStorageTests : public QObject {
	Q_OBJECT
private slots:
	void save_withNullId_fails();
	void save_survives_aJsonStorageLayerReopen();
	void save_insertsANewEntity();
	void save_updatesAnExistingEntity();
	void exists_reflectsWhatWasSaved();
	void load_missingId_returnsNullptr();
	void load_roundTripsTheEntity();
	void loadAll_returnsEveryStoredEntity();
	void remove_deletesTheEntity();
};

void EntityStorageTests::save_withNullId_fails() {
	InMemoryStorageLayer layer;
	QVERIFY(layer.open());
	EntityStorage<Account> storage(layer, QStringLiteral("accounts"));

	Account account;
	account.setBank(QStringLiteral("BNP"));
	QVERIFY(!storage.save(account));
}

// Proves EntityStorage is genuinely backend-agnostic: identical calls, only
// the AbstractStorageLayer changes, and the account survives a full
// close/reopen against a fresh layer instance backed by the same file.
void EntityStorageTests::save_survives_aJsonStorageLayerReopen() {
	QTemporaryDir dir;
	QVERIFY(dir.isValid());
	const QString filePath = dir.filePath(QStringLiteral("storage.json"));

	QUuid id;
	{
		JsonStorageLayer layer(filePath);
		QVERIFY(layer.open());
		EntityStorage<Account> storage(layer, QStringLiteral("accounts"));

		Account account;
		account.setId(QUuid::createUuid());
		account.setBank(QStringLiteral("BNP"));
		id = account.id();
		QVERIFY(storage.save(account));
	}

	JsonStorageLayer reopened(filePath);
	QVERIFY(reopened.open());
	EntityStorage<Account> storage(reopened, QStringLiteral("accounts"));

	const QScopedPointer<Account> reloaded(storage.load(id));
	QVERIFY(reloaded);
	QCOMPARE(reloaded->id(), id);
	QCOMPARE(reloaded->bank(), QStringLiteral("BNP"));
}

void EntityStorageTests::save_insertsANewEntity() {
	InMemoryStorageLayer layer;
	QVERIFY(layer.open());
	EntityStorage<Account> storage(layer, QStringLiteral("accounts"));

	Account account;
	account.setId(QUuid::createUuid());
	account.setBank(QStringLiteral("BNP"));
	QVERIFY(storage.save(account));
	QVERIFY(storage.exists(account.id()));
}

void EntityStorageTests::save_updatesAnExistingEntity() {
	InMemoryStorageLayer layer;
	QVERIFY(layer.open());
	EntityStorage<Account> storage(layer, QStringLiteral("accounts"));

	Account account;
	account.setId(QUuid::createUuid());
	account.setBank(QStringLiteral("BNP"));
	QVERIFY(storage.save(account));

	account.setBank(QStringLiteral("BNP Paribas"));
	QVERIFY(storage.save(account));

	const QScopedPointer<Account> reloaded(storage.load(account.id()));
	QVERIFY(reloaded);
	QCOMPARE(reloaded->bank(), QStringLiteral("BNP Paribas"));

	const StorageResult all = layer.execute(SelectCommand{QStringLiteral("accounts"), {}, {}});
	QCOMPARE(all.records.size(), 1); // updated in place, not duplicated
}

void EntityStorageTests::exists_reflectsWhatWasSaved() {
	InMemoryStorageLayer layer;
	QVERIFY(layer.open());
	EntityStorage<Account> storage(layer, QStringLiteral("accounts"));

	QVERIFY(!storage.exists(QUuid::createUuid()));

	Account account;
	account.setId(QUuid::createUuid());
	QVERIFY(storage.save(account));
	QVERIFY(storage.exists(account.id()));
}

void EntityStorageTests::load_missingId_returnsNullptr() {
	InMemoryStorageLayer layer;
	QVERIFY(layer.open());
	EntityStorage<Account> storage(layer, QStringLiteral("accounts"));

	QVERIFY(!storage.load(QUuid::createUuid()));
}

void EntityStorageTests::load_roundTripsTheEntity() {
	InMemoryStorageLayer layer;
	QVERIFY(layer.open());
	EntityStorage<Account> storage(layer, QStringLiteral("accounts"));

	Account account;
	account.setId(QUuid::createUuid());
	account.setNumber(QStringLiteral("FR76"));
	account.setBank(QStringLiteral("BNP"));
	account.setInterest(1.5);
	account.setDescription(QStringLiteral("Main account"));
	QVERIFY(storage.save(account));

	const QScopedPointer<Account> reloaded(storage.load(account.id()));
	QVERIFY(reloaded);
	QCOMPARE(reloaded->id(), account.id());
	QCOMPARE(reloaded->number(), account.number());
	QCOMPARE(reloaded->bank(), account.bank());
	QCOMPARE(reloaded->interest(), account.interest());
	QCOMPARE(reloaded->description(), account.description());
}

void EntityStorageTests::loadAll_returnsEveryStoredEntity() {
	InMemoryStorageLayer layer;
	QVERIFY(layer.open());
	EntityStorage<Account> storage(layer, QStringLiteral("accounts"));

	Account first;
	first.setId(QUuid::createUuid());
	Account second;
	second.setId(QUuid::createUuid());
	QVERIFY(storage.save(first));
	QVERIFY(storage.save(second));

	const QList<Account *> all = storage.loadAll();
	QCOMPARE(all.size(), 2);
	qDeleteAll(all);
}

void EntityStorageTests::remove_deletesTheEntity() {
	InMemoryStorageLayer layer;
	QVERIFY(layer.open());
	EntityStorage<Account> storage(layer, QStringLiteral("accounts"));

	Account account;
	account.setId(QUuid::createUuid());
	QVERIFY(storage.save(account));

	QVERIFY(storage.remove(account.id()));
	QVERIFY(!storage.exists(account.id()));
	// DELETE on an already-gone id still "succeeds" with 0 rows affected,
	// same idempotent-no-op semantics as SQL DELETE.
	QVERIFY(storage.remove(account.id()));
}

QTEST_APPLESS_MAIN(EntityStorageTests)

#include "tst_entitystorage.moc"
