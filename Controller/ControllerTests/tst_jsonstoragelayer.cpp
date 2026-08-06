#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTemporaryDir>
#include <QtTest>

#include "Controller/jsonstoragelayer.h"

using namespace Controller;

class JsonStorageLayerTests : public QObject {
	Q_OBJECT
private slots:
	void init();
	void cleanup();

	void open_onMissingFile_startsEmptyAndSucceeds();
	void open_thenClose_toggleIsOpen();
	void execute_fails_when_not_open();
	void insert_persists_toDisk();
	void insert_thenSelect_roundtrip();
	void update_persists_toDisk();
	void remove_persists_toDisk();
	void reopeningTheSameFile_restoresData();
	void aSecondLayer_canReadWhatTheFirstWrote();
	void open_onCorruptFile_fails();
	void open_onNonObjectJson_fails();

private:
	QTemporaryDir m_dir;
	QString m_filePath;
};

void JsonStorageLayerTests::init() {
	QVERIFY(m_dir.isValid());
	m_filePath = m_dir.filePath(QStringLiteral("storage.json"));
}

void JsonStorageLayerTests::cleanup() {
	QFile::remove(m_filePath);
}

void JsonStorageLayerTests::open_onMissingFile_startsEmptyAndSucceeds() {
	JsonStorageLayer layer(m_filePath);
	QVERIFY(!QFile::exists(m_filePath));
	QVERIFY(layer.open());
	QVERIFY(layer.isOpen());

	const StorageResult result = layer.execute(SelectCommand{QStringLiteral("accounts"), {}, {}});
	QVERIFY(result.success);
	QVERIFY(result.records.isEmpty());
}

void JsonStorageLayerTests::open_thenClose_toggleIsOpen() {
	JsonStorageLayer layer(m_filePath);
	QVERIFY(!layer.isOpen());
	QVERIFY(layer.open());
	QVERIFY(layer.isOpen());
	layer.close();
	QVERIFY(!layer.isOpen());
}

void JsonStorageLayerTests::execute_fails_when_not_open() {
	JsonStorageLayer layer(m_filePath);
	const StorageResult result = layer.execute(SelectCommand{QStringLiteral("accounts"), {}, {}});
	QVERIFY(!result.success);
	QVERIFY(!result.error.isEmpty());
}

void JsonStorageLayerTests::insert_persists_toDisk() {
	JsonStorageLayer layer(m_filePath);
	QVERIFY(layer.open());

	QVERIFY(!QFile::exists(m_filePath));

	const StorageResult inserted = layer.execute(InsertCommand{
	    QStringLiteral("accounts"),
	    {{QStringLiteral("id"), QStringLiteral("a1")}, {QStringLiteral("bank"), QStringLiteral("BNP")}}});
	QVERIFY(inserted.success);
	QCOMPARE(inserted.affected, 1);

	QVERIFY(QFile::exists(m_filePath));

	QFile file(m_filePath);
	QVERIFY(file.open(QIODevice::ReadOnly));
	const QJsonDocument document = QJsonDocument::fromJson(file.readAll());
	QVERIFY(document.isObject());
	const QJsonArray accounts = document.object().value(QStringLiteral("accounts")).toArray();
	QCOMPARE(accounts.size(), 1);
	QCOMPARE(accounts.first().toObject().value(QStringLiteral("bank")).toString(), QStringLiteral("BNP"));
}

void JsonStorageLayerTests::insert_thenSelect_roundtrip() {
	JsonStorageLayer layer(m_filePath);
	QVERIFY(layer.open());

	layer.execute(InsertCommand{QStringLiteral("accounts"),
	                            {{QStringLiteral("id"), QStringLiteral("a1")}, {QStringLiteral("bank"), QStringLiteral("BNP")}}});

	const StorageResult selected = layer.execute(SelectCommand{QStringLiteral("accounts"), {}, {}});
	QVERIFY(selected.success);
	QCOMPARE(selected.records.size(), 1);
	QCOMPARE(selected.records.first().value(QStringLiteral("bank")).toString(), QStringLiteral("BNP"));
}

void JsonStorageLayerTests::update_persists_toDisk() {
	JsonStorageLayer layer(m_filePath);
	QVERIFY(layer.open());
	layer.execute(InsertCommand{QStringLiteral("accounts"),
	                            {{QStringLiteral("id"), QStringLiteral("a1")}, {QStringLiteral("bank"), QStringLiteral("BNP")}}});

	const StorageResult updated = layer.execute(UpdateCommand{
	    QStringLiteral("accounts"),
	    {{QStringLiteral("bank"), QStringLiteral("BNP Paribas")}},
	    {{QStringLiteral("id"), CompareOp::Equal, QStringLiteral("a1")}}});
	QVERIFY(updated.success);
	QCOMPARE(updated.affected, 1);

	JsonStorageLayer reopened(m_filePath);
	QVERIFY(reopened.open());
	const StorageResult selected = reopened.execute(SelectCommand{QStringLiteral("accounts"), {}, {}});
	QCOMPARE(selected.records.first().value(QStringLiteral("bank")).toString(), QStringLiteral("BNP Paribas"));
}

void JsonStorageLayerTests::remove_persists_toDisk() {
	JsonStorageLayer layer(m_filePath);
	QVERIFY(layer.open());
	layer.execute(InsertCommand{QStringLiteral("accounts"), {{QStringLiteral("id"), QStringLiteral("a1")}}});
	layer.execute(InsertCommand{QStringLiteral("accounts"), {{QStringLiteral("id"), QStringLiteral("a2")}}});

	const StorageResult deleted = layer.execute(DeleteCommand{
	    QStringLiteral("accounts"),
	    {{QStringLiteral("id"), CompareOp::Equal, QStringLiteral("a1")}}});
	QVERIFY(deleted.success);
	QCOMPARE(deleted.affected, 1);

	JsonStorageLayer reopened(m_filePath);
	QVERIFY(reopened.open());
	const StorageResult remaining = reopened.execute(SelectCommand{QStringLiteral("accounts"), {}, {}});
	QCOMPARE(remaining.records.size(), 1);
	QCOMPARE(remaining.records.first().value(QStringLiteral("id")).toString(), QStringLiteral("a2"));
}

void JsonStorageLayerTests::reopeningTheSameFile_restoresData() {
	{
		JsonStorageLayer layer(m_filePath);
		QVERIFY(layer.open());
		layer.execute(InsertCommand{QStringLiteral("profiles"),
		                            {{QStringLiteral("id"), QStringLiteral("p1")}, {QStringLiteral("first_name"), QStringLiteral("Arnaud")}}});
		layer.close();
	}

	JsonStorageLayer reopened(m_filePath);
	QVERIFY(reopened.open());
	const StorageResult selected = reopened.execute(SelectCommand{QStringLiteral("profiles"), {}, {}});
	QVERIFY(selected.success);
	QCOMPARE(selected.records.size(), 1);
	QCOMPARE(selected.records.first().value(QStringLiteral("first_name")).toString(), QStringLiteral("Arnaud"));
}

void JsonStorageLayerTests::aSecondLayer_canReadWhatTheFirstWrote() {
	JsonStorageLayer writer(m_filePath);
	QVERIFY(writer.open());
	writer.execute(InsertCommand{QStringLiteral("accounts"), {{QStringLiteral("id"), QStringLiteral("a1")}}});

	JsonStorageLayer reader(m_filePath);
	QVERIFY(reader.open());
	const StorageResult selected = reader.execute(SelectCommand{QStringLiteral("accounts"), {}, {}});
	QCOMPARE(selected.records.size(), 1);
}

void JsonStorageLayerTests::open_onCorruptFile_fails() {
	QFile file(m_filePath);
	QVERIFY(file.open(QIODevice::WriteOnly));
	file.write(QByteArrayLiteral("{ not valid json"));
	file.close();

	JsonStorageLayer layer(m_filePath);
	QVERIFY(!layer.open());
	QVERIFY(!layer.isOpen());
}

void JsonStorageLayerTests::open_onNonObjectJson_fails() {
	QFile file(m_filePath);
	QVERIFY(file.open(QIODevice::WriteOnly));
	file.write(QByteArrayLiteral("[1, 2, 3]"));
	file.close();

	JsonStorageLayer layer(m_filePath);
	QVERIFY(!layer.open());
}

QTEST_APPLESS_MAIN(JsonStorageLayerTests)

#include "tst_jsonstoragelayer.moc"
