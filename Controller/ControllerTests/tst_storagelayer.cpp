#include <QtTest>
#include <type_traits>

#include "Controller/abstractstoragelayer.h"
#include "Controller/inmemorystoragelayer.h"

using namespace Controller;

class StorageLayerTests : public QObject {
	Q_OBJECT
private slots:
	void abstractStorageLayer_is_abstract();
	void execute_fails_when_not_open();
	void insert_then_select_roundtrip();
	void select_filters_with_equality();
	void select_projects_requested_columns();
	void select_supports_ordering_and_like();
	void update_modifies_matching_rows();
	void remove_deletes_matching_rows();
	void profiles_and_accounts_scenario();
};

void StorageLayerTests::abstractStorageLayer_is_abstract() {
	QVERIFY(std::is_abstract_v<AbstractStorageLayer>);
}

void StorageLayerTests::execute_fails_when_not_open() {
	InMemoryStorageLayer layer;
	QVERIFY(!layer.isOpen());

	const StorageResult result = layer.execute(SelectCommand{QStringLiteral("accounts"), {}, {}});
	QVERIFY(!result.success);
	QVERIFY(!result.error.isEmpty());
}

void StorageLayerTests::insert_then_select_roundtrip() {
	InMemoryStorageLayer layer;
	QVERIFY(layer.open());

	const StorageResult inserted = layer.execute(InsertCommand{
	    QStringLiteral("accounts"),
	    {{QStringLiteral("id"), QStringLiteral("a1")}, {QStringLiteral("bank"), QStringLiteral("BNP")}}});
	QVERIFY(inserted.success);
	QCOMPARE(inserted.affected, 1);

	const StorageResult selected = layer.execute(SelectCommand{QStringLiteral("accounts"), {}, {}});
	QVERIFY(selected.success);
	QCOMPARE(selected.records.size(), 1);
	QCOMPARE(selected.records.first().value(QStringLiteral("bank")).toString(), QStringLiteral("BNP"));
}

void StorageLayerTests::select_filters_with_equality() {
	InMemoryStorageLayer layer;
	QVERIFY(layer.open());

	layer.execute(InsertCommand{QStringLiteral("accounts"),
	                            {{QStringLiteral("id"), QStringLiteral("a1")}, {QStringLiteral("bank"), QStringLiteral("BNP")}}});
	layer.execute(InsertCommand{QStringLiteral("accounts"),
	                            {{QStringLiteral("id"), QStringLiteral("a2")}, {QStringLiteral("bank"), QStringLiteral("CA")}}});

	const StorageResult result = layer.execute(SelectCommand{
	    QStringLiteral("accounts"),
	    {{QStringLiteral("bank"), CompareOp::Equal, QStringLiteral("CA")}},
	    {}});
	QVERIFY(result.success);
	QCOMPARE(result.records.size(), 1);
	QCOMPARE(result.records.first().value(QStringLiteral("id")).toString(), QStringLiteral("a2"));
}

void StorageLayerTests::select_projects_requested_columns() {
	InMemoryStorageLayer layer;
	QVERIFY(layer.open());

	layer.execute(InsertCommand{QStringLiteral("accounts"),
	                            {{QStringLiteral("id"), QStringLiteral("a1")},
	                             {QStringLiteral("bank"), QStringLiteral("BNP")},
	                             {QStringLiteral("interest"), 1.5}}});

	const StorageResult result = layer.execute(SelectCommand{
	    QStringLiteral("accounts"), {}, {QStringLiteral("id")}});
	QVERIFY(result.success);
	QCOMPARE(result.records.size(), 1);
	const Record row = result.records.first();
	QCOMPARE(row.size(), 1);
	QVERIFY(row.contains(QStringLiteral("id")));
	QVERIFY(!row.contains(QStringLiteral("bank")));
}

void StorageLayerTests::select_supports_ordering_and_like() {
	InMemoryStorageLayer layer;
	QVERIFY(layer.open());

	layer.execute(InsertCommand{QStringLiteral("accounts"),
	                            {{QStringLiteral("bank"), QStringLiteral("Boursorama")}, {QStringLiteral("interest"), 0.5}}});
	layer.execute(InsertCommand{QStringLiteral("accounts"),
	                            {{QStringLiteral("bank"), QStringLiteral("BNP")}, {QStringLiteral("interest"), 2.0}}});

	const StorageResult byRate = layer.execute(SelectCommand{
	    QStringLiteral("accounts"),
	    {{QStringLiteral("interest"), CompareOp::GreaterThan, 1.0}},
	    {}});
	QVERIFY(byRate.success);
	QCOMPARE(byRate.records.size(), 1);
	QCOMPARE(byRate.records.first().value(QStringLiteral("bank")).toString(), QStringLiteral("BNP"));

	const StorageResult byName = layer.execute(SelectCommand{
	    QStringLiteral("accounts"),
	    {{QStringLiteral("bank"), CompareOp::Like, QStringLiteral("b%")}},
	    {}});
	QVERIFY(byName.success);
	QCOMPARE(byName.records.size(), 2); // case-insensitive: both start with 'B'
}

void StorageLayerTests::update_modifies_matching_rows() {
	InMemoryStorageLayer layer;
	QVERIFY(layer.open());

	layer.execute(InsertCommand{QStringLiteral("accounts"),
	                            {{QStringLiteral("id"), QStringLiteral("a1")}, {QStringLiteral("bank"), QStringLiteral("BNP")}}});

	const StorageResult updated = layer.execute(UpdateCommand{
	    QStringLiteral("accounts"),
	    {{QStringLiteral("bank"), QStringLiteral("BNP Paribas")}},
	    {{QStringLiteral("id"), CompareOp::Equal, QStringLiteral("a1")}}});
	QVERIFY(updated.success);
	QCOMPARE(updated.affected, 1);

	const StorageResult selected = layer.execute(SelectCommand{QStringLiteral("accounts"), {}, {}});
	QCOMPARE(selected.records.first().value(QStringLiteral("bank")).toString(), QStringLiteral("BNP Paribas"));
}

void StorageLayerTests::remove_deletes_matching_rows() {
	InMemoryStorageLayer layer;
	QVERIFY(layer.open());

	layer.execute(InsertCommand{QStringLiteral("accounts"), {{QStringLiteral("id"), QStringLiteral("a1")}}});
	layer.execute(InsertCommand{QStringLiteral("accounts"), {{QStringLiteral("id"), QStringLiteral("a2")}}});

	const StorageResult deleted = layer.execute(DeleteCommand{
	    QStringLiteral("accounts"),
	    {{QStringLiteral("id"), CompareOp::Equal, QStringLiteral("a1")}}});
	QVERIFY(deleted.success);
	QCOMPARE(deleted.affected, 1);

	const StorageResult remaining = layer.execute(SelectCommand{QStringLiteral("accounts"), {}, {}});
	QCOMPARE(remaining.records.size(), 1);
	QCOMPARE(remaining.records.first().value(QStringLiteral("id")).toString(), QStringLiteral("a2"));
}

// End-to-end on the domain the schema was designed for: one profile owning two
// accounts, reached through the (profile_id) link as the SQLite schema models it.
void StorageLayerTests::profiles_and_accounts_scenario() {
	InMemoryStorageLayer layer;
	QVERIFY(layer.open());

	layer.execute(InsertCommand{QStringLiteral("profiles"),
	                            {{QStringLiteral("id"), QStringLiteral("p1")},
	                             {QStringLiteral("first_name"), QStringLiteral("Arnaud")},
	                             {QStringLiteral("last_name"), QStringLiteral("Duhamel")}}});
	layer.execute(InsertCommand{QStringLiteral("accounts"),
	                            {{QStringLiteral("id"), QStringLiteral("a1")}, {QStringLiteral("profile_id"), QStringLiteral("p1")}}});
	layer.execute(InsertCommand{QStringLiteral("accounts"),
	                            {{QStringLiteral("id"), QStringLiteral("a2")}, {QStringLiteral("profile_id"), QStringLiteral("p1")}}});

	const StorageResult owned = layer.execute(SelectCommand{
	    QStringLiteral("accounts"),
	    {{QStringLiteral("profile_id"), CompareOp::Equal, QStringLiteral("p1")}},
	    {QStringLiteral("id")}});
	QVERIFY(owned.success);
	QCOMPARE(owned.records.size(), 2);

	// Data survives a close()/open() cycle (only the open flag toggles).
	layer.close();
	QVERIFY(!layer.isOpen());
	QVERIFY(layer.open());
	const StorageResult afterReopen = layer.execute(SelectCommand{QStringLiteral("profiles"), {}, {}});
	QCOMPARE(afterReopen.records.size(), 1);
}

QTEST_APPLESS_MAIN(StorageLayerTests)

#include "tst_storagelayer.moc"
