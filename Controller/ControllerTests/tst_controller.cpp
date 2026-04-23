#include <QtTest>
#include <type_traits>

#include "Controller/controller.h"

namespace {

class InMemoryDataStorage final : public Controller::AbstractDataStorage {
public:
	using Controller::AbstractDataStorage::AbstractDataStorage;

	bool exists() const override {
		return m_hasData;
	}

	QByteArray load() const override {
		return m_serializedData;
	}

	bool save(const QByteArray &serializedData) override {
		m_serializedData = serializedData;
		m_hasData = true;
		return true;
	}

	bool clear() override {
		m_serializedData.clear();
		m_hasData = false;
		return true;
	}

private:
	QByteArray m_serializedData;
	bool m_hasData = false;
};

} // namespace

class ControllerTests : public QObject {
	Q_OBJECT
private slots:
	void name_returns_Controller();
	void abstractDataStorage_is_abstract();
	void abstractDataStorage_manages_storageLocation();
	void abstractDataStorage_contract_can_be_implemented();
};

void ControllerTests::name_returns_Controller() {
	Controller::ControllerApi api;
	QCOMPARE(api.name(), QStringLiteral("OpenAccount Controller"));
}

void ControllerTests::abstractDataStorage_is_abstract() {
	QVERIFY(std::is_abstract_v<Controller::AbstractDataStorage>);
}

void ControllerTests::abstractDataStorage_manages_storageLocation() {
	InMemoryDataStorage storage(QStringLiteral("memory://initial"));
	QCOMPARE(storage.storageLocation(), QStringLiteral("memory://initial"));

	storage.setStorageLocation(QStringLiteral("memory://updated"));
	QCOMPARE(storage.storageLocation(), QStringLiteral("memory://updated"));
}

void ControllerTests::abstractDataStorage_contract_can_be_implemented() {
	InMemoryDataStorage storage;
	QVERIFY(!storage.exists());
	QCOMPARE(storage.load(), QByteArray());

	QVERIFY(storage.save(QByteArrayLiteral("serialized-payload")));
	QVERIFY(storage.exists());
	QCOMPARE(storage.load(), QByteArrayLiteral("serialized-payload"));

	QVERIFY(storage.clear());
	QVERIFY(!storage.exists());
	QCOMPARE(storage.load(), QByteArray());
}

QTEST_APPLESS_MAIN(ControllerTests)

#include "tst_controller.moc"

