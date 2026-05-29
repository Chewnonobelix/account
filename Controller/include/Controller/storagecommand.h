#pragma once

#include <QList>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QVariantMap>
#include <utility>
#include <variant>

#include "controller_global.h"

namespace Controller {

// A single record: column/field name -> value. Backend-agnostic on purpose so
// that an SQLite row, a JSON object or an in-memory map all map onto the same
// shape.
using Record = QVariantMap;

// Comparison operators usable inside a WHERE condition. Names mirror the SQL
// operators they stand for.
enum class CompareOp {
	Equal,          // field = value
	NotEqual,       // field <> value
	LessThan,       // field < value
	LessOrEqual,    // field <= value
	GreaterThan,    // field > value
	GreaterOrEqual, // field >= value
	Like,           // field LIKE value ('%' and '_' wildcards, case-insensitive)
};

// One filter condition: <field> <op> <value>.
struct Condition {
	QString field;
	CompareOp op = CompareOp::Equal;
	QVariant value;
};

// A set of conditions combined with logical AND. An empty Criteria matches
// every row (i.e. no WHERE clause).
using Criteria = QList<Condition>;

// SELECT [columns] FROM <table> [WHERE <where>]. Empty `columns` means "all".
struct SelectCommand {
	QString table;
	Criteria where;
	QStringList columns;
};

// INSERT INTO <table> (...) VALUES (...).
struct InsertCommand {
	QString table;
	Record row;
};

// UPDATE <table> SET <values> [WHERE <where>].
struct UpdateCommand {
	QString table;
	Record values;
	Criteria where;
};

// DELETE FROM <table> [WHERE <where>].
struct DeleteCommand {
	QString table;
	Criteria where;
};

// Any storage command. Callers build one of the structs above and submit it to
// AbstractStorageLayer::execute(); a SelectCommand/InsertCommand/... converts to
// this variant implicitly, so `execute(SelectCommand{...})` just works.
using Command = std::variant<SelectCommand, InsertCommand, UpdateCommand, DeleteCommand>;

// Outcome of executing a command. Implementations never throw: a failure is
// reported through `success == false` and a human-readable `error`.
struct StorageResult {
	bool success = false;
	QString error;         // populated only when success == false
	QList<Record> records; // rows returned by a SELECT
	int affected = 0;      // rows touched by INSERT / UPDATE / DELETE

	[[nodiscard]] static StorageResult ok() {
		StorageResult result;
		result.success = true;
		return result;
	}

	[[nodiscard]] static StorageResult fail(QString message) {
		StorageResult result;
		result.success = false;
		result.error = std::move(message);
		return result;
	}
};

} // namespace Controller
