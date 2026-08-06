#include <QtTest>

#include "Controller/frequencycontroller.h"
#include "Controller/inmemorystoragelayer.h"

using namespace Controller;

class FrequencyControllerTests : public QObject {
	Q_OBJECT
private slots:
	void isAnAbstractController();
	void saveThenLoad_roundTrips();
	void loadAll_returnsEveryFrequency();
	void remove_deletesTheFrequency();
};

void FrequencyControllerTests::isAnAbstractController() {
	InMemoryStorageLayer layer;
	FrequencyController controller(layer);
	QVERIFY(qobject_cast<AbstractController *>(&controller) != nullptr);
}

void FrequencyControllerTests::saveThenLoad_roundTrips() {
	InMemoryStorageLayer layer;
	QVERIFY(layer.open());
	FrequencyController controller(layer);

	const FrequencyPtr frequency = FrequencyPtr::create();
	frequency->setId(QUuid::createUuid());
	frequency->setFrequency(OpenAccountEnums::Frequency::Monthly);
	frequency->setCustomIntervalDays(5);
	QVERIFY(controller.save(frequency));
	QVERIFY(controller.exists(frequency->id()));

	const FrequencyPtr reloaded = controller.load(frequency->id());
	QVERIFY(reloaded);
	QCOMPARE(reloaded->frequency(), OpenAccountEnums::Frequency::Monthly);
	QCOMPARE(reloaded->customIntervalDays(), 5);
}

void FrequencyControllerTests::loadAll_returnsEveryFrequency() {
	InMemoryStorageLayer layer;
	QVERIFY(layer.open());
	FrequencyController controller(layer);

	const FrequencyPtr first = FrequencyPtr::create();
	first->setId(QUuid::createUuid());
	const FrequencyPtr second = FrequencyPtr::create();
	second->setId(QUuid::createUuid());
	QVERIFY(controller.save(first));
	QVERIFY(controller.save(second));

	QCOMPARE(controller.loadAll().size(), 2);
}

void FrequencyControllerTests::remove_deletesTheFrequency() {
	InMemoryStorageLayer layer;
	QVERIFY(layer.open());
	FrequencyController controller(layer);

	const FrequencyPtr frequency = FrequencyPtr::create();
	frequency->setId(QUuid::createUuid());
	QVERIFY(controller.save(frequency));
	QVERIFY(controller.remove(frequency->id()));
	QVERIFY(!controller.exists(frequency->id()));
}

QTEST_APPLESS_MAIN(FrequencyControllerTests)

#include "tst_frequencycontroller.moc"
