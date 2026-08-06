#include "Controller/inmemorystoragelayer.h"

#include <QChar>
#include <QLatin1Char>
#include <QMetaType>
#include <QRegularExpression>

#include <utility>
#include <variant>

using namespace Controller;

namespace {

// Three-way compare for ordering operators. Numbers are compared numerically,
// everything else by its string form (which keeps ISO dates ordered correctly).
int compareVariants(const QVariant &lhs, const QVariant &rhs) {
	const auto isNumeric = [](const QVariant &v) {
		return v.typeId() != QMetaType::QString && v.typeId() != QMetaType::QByteArray
		       && v.canConvert<double>();
	};

	if (isNumeric(lhs) && isNumeric(rhs)) {
		const double a = lhs.toDouble();
		const double b = rhs.toDouble();
		if (a < b) {
			return -1;
		}
		if (a > b) {
			return 1;
		}
		return 0;
	}
	return QString::compare(lhs.toString(), rhs.toString());
}

// SQL LIKE semantics: '%' matches any run of characters, '_' matches one, the
// rest is literal, matching is case-insensitive (as SQLite does for ASCII).
bool likeMatches(const QString &text, const QString &pattern) {
	QString expression;
	expression.reserve(pattern.size() * 2 + 2);
	expression += QLatin1Char('^');
	for (const QChar ch : pattern) {
		if (ch == QLatin1Char('%')) {
			expression += QStringLiteral(".*");
		} else if (ch == QLatin1Char('_')) {
			expression += QLatin1Char('.');
		} else {
			expression += QRegularExpression::escape(QString(ch));
		}
	}
	expression += QLatin1Char('$');

	const QRegularExpression regex(expression, QRegularExpression::CaseInsensitiveOption);
	return regex.match(text).hasMatch();
}

bool conditionHolds(const QVariant &fieldValue, CompareOp op, const QVariant &target) {
	switch (op) {
	case CompareOp::Equal:
		return fieldValue == target;
	case CompareOp::NotEqual:
		return fieldValue != target;
	case CompareOp::Like:
		return likeMatches(fieldValue.toString(), target.toString());
	case CompareOp::LessThan:
		return compareVariants(fieldValue, target) < 0;
	case CompareOp::LessOrEqual:
		return compareVariants(fieldValue, target) <= 0;
	case CompareOp::GreaterThan:
		return compareVariants(fieldValue, target) > 0;
	case CompareOp::GreaterOrEqual:
		return compareVariants(fieldValue, target) >= 0;
	}
	return false;
}

// A row matches when every condition holds (AND). Empty criteria match all.
bool matches(const Record &row, const Criteria &where) {
	for (const Condition &condition : where) {
		if (!conditionHolds(row.value(condition.field), condition.op, condition.value)) {
			return false;
		}
	}
	return true;
}

// Keep only the requested columns; an empty list means "all columns".
Record project(const Record &row, const QStringList &columns) {
	if (columns.isEmpty()) {
		return row;
	}
	Record projected;
	for (const QString &column : columns) {
		if (row.contains(column)) {
			projected.insert(column, row.value(column));
		}
	}
	return projected;
}

} // namespace

bool InMemoryStorageLayer::open() {
	m_open = true;
	return true;
}

void InMemoryStorageLayer::close() {
	m_open = false;
}

bool InMemoryStorageLayer::isOpen() const {
	return m_open;
}

StorageResult InMemoryStorageLayer::execute(const Command &command) {
	if (!m_open) {
		return StorageResult::fail(QStringLiteral("storage layer is not open"));
	}
	return std::visit([this](const auto &concrete) { return run(concrete); }, command);
}

QHash<QString, QList<Record>> InMemoryStorageLayer::tables() const {
	return m_tables;
}

void InMemoryStorageLayer::setTables(QHash<QString, QList<Record>> tables) {
	m_tables = std::move(tables);
}

StorageResult InMemoryStorageLayer::run(const SelectCommand &command) {
	StorageResult result = StorageResult::ok();
	const QList<Record> &rows = m_tables.value(command.table);
	for (const Record &row : rows) {
		if (matches(row, command.where)) {
			result.records.append(project(row, command.columns));
		}
	}
	return result;
}

StorageResult InMemoryStorageLayer::run(const InsertCommand &command) {
	if (command.table.isEmpty()) {
		return StorageResult::fail(QStringLiteral("INSERT requires a table name"));
	}
	m_tables[command.table].append(command.row);

	StorageResult result = StorageResult::ok();
	result.affected = 1;
	return result;
}

StorageResult InMemoryStorageLayer::run(const UpdateCommand &command) {
	StorageResult result = StorageResult::ok();
	auto it = m_tables.find(command.table);
	if (it == m_tables.end()) {
		return result; // unknown table: nothing to update
	}

	for (Record &row : *it) {
		if (!matches(row, command.where)) {
			continue;
		}
		for (auto field = command.values.cbegin(); field != command.values.cend(); ++field) {
			row.insert(field.key(), field.value());
		}
		++result.affected;
	}
	return result;
}

StorageResult InMemoryStorageLayer::run(const DeleteCommand &command) {
	StorageResult result = StorageResult::ok();
	auto it = m_tables.find(command.table);
	if (it == m_tables.end()) {
		return result; // unknown table: nothing to delete
	}

	QList<Record> &rows = *it;
	const qsizetype before = rows.size();
	for (qsizetype i = rows.size() - 1; i >= 0; --i) {
		if (matches(rows.at(i), command.where)) {
			rows.removeAt(i);
		}
	}
	result.affected = static_cast<int>(before - rows.size());
	return result;
}
