#include <QtTest>

#include "Controller/inmemorystoragelayer.h"
#include "Controller/profilecontroller.h"

using namespace Controller;

class ProfileControllerTests : public QObject {
	Q_OBJECT
private slots:
	void isAnAbstractController();
	void saveThenLoad_roundTrips();
	void loadAll_returnsEveryProfile();
	void remove_deletesTheProfile();
};

void ProfileControllerTests::isAnAbstractController() {
	InMemoryStorageLayer layer;
	ProfileController controller(layer);
	QVERIFY(qobject_cast<AbstractController *>(&controller) != nullptr);
}

void ProfileControllerTests::saveThenLoad_roundTrips() {
	InMemoryStorageLayer layer;
	QVERIFY(layer.open());
	ProfileController controller(layer);

	Profile profile;
	profile.setId(QUuid::createUuid());
	profile.setFirstName(QStringLiteral("Arnaud"));
	profile.setLastName(QStringLiteral("Duhamel"));
	QVERIFY(controller.save(&profile));
	QVERIFY(controller.exists(profile.id()));

	const QScopedPointer<Profile> reloaded(controller.load(profile.id()));
	QVERIFY(reloaded);
	QCOMPARE(reloaded->firstName(), QStringLiteral("Arnaud"));
	QCOMPARE(reloaded->lastName(), QStringLiteral("Duhamel"));
}

void ProfileControllerTests::loadAll_returnsEveryProfile() {
	InMemoryStorageLayer layer;
	QVERIFY(layer.open());
	ProfileController controller(layer);

	Profile first;
	first.setId(QUuid::createUuid());
	Profile second;
	second.setId(QUuid::createUuid());
	QVERIFY(controller.save(&first));
	QVERIFY(controller.save(&second));

	const QList<Profile *> all = controller.loadAll();
	QCOMPARE(all.size(), 2);
	qDeleteAll(all);
}

void ProfileControllerTests::remove_deletesTheProfile() {
	InMemoryStorageLayer layer;
	QVERIFY(layer.open());
	ProfileController controller(layer);

	Profile profile;
	profile.setId(QUuid::createUuid());
	QVERIFY(controller.save(&profile));
	QVERIFY(controller.remove(profile.id()));
	QVERIFY(!controller.exists(profile.id()));
}

QTEST_APPLESS_MAIN(ProfileControllerTests)

#include "tst_profilecontroller.moc"
