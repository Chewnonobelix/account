#pragma once

#include <QJsonObject>
#include <QList>
#include <QObject>
#include <QString>
#include <QUuid>
#include <utility>

#include "abstractstoragelayer.h"

namespace Controller {

// Generic save/load/remove logic shared by every concrete *Controller
// (AccountController, TransactionController, ...).
//
// Every persisted business entity (Account, Profile, Transaction, Category,
// Debt, Frequency, Tricount, Budget) already round-trips itself through
// toJson()/fromJson() (see the Model layer's JSON serialization tests), and
// all follow the same shape:
//   - T(const QJsonObject&, QObject* parent = nullptr) reconstructs one
//   - T::toJson() const -> QJsonObject serializes one
//   - T::id() const -> QUuid
// so persistence reduces to storing/restoring that JSON as a Record under
// <table, id>, with no per-entity column mapping to hand-write. A concrete
// controller is then a thin wrapper pinning down the table name (and, for
// entities owned via QSharedPointer, wrapping the returned raw pointer).
template <typename T> class EntityStorage {
public:
	EntityStorage(AbstractStorageLayer &layer, QString table) : m_layer(layer), m_table(std::move(table)) {}

	[[nodiscard]] const QString &table() const { return m_table; }

	[[nodiscard]] bool exists(const QUuid &id) const {
		if (id.isNull()) {
			return false;
		}
		const StorageResult result = m_layer.execute(
		    SelectCommand{m_table, {idCondition(id)}, {QStringLiteral("id")}});
		return result.success && !result.records.isEmpty();
	}

	// INSERTs a never-seen id, UPDATEs an existing one. False on a null id or
	// a storage failure.
	bool save(const T &entity) {
		const QUuid id = entity.id();
		if (id.isNull()) {
			return false;
		}

		Record record = QJsonObject(entity.toJson()).toVariantMap();
		// Entities disagree on the JSON string form of their own id (most
		// keep QUuid::toString()'s braces via the generic MetaData
		// serialization; Category deliberately strips them - see
		// Category::toJson()). Normalize what actually lands in storage to
		// one canonical form so id-based lookups are reliable regardless of
		// which entity type is stored; QUuid's own string parsing accepts
		// either form back on the way out via fromJson().
		record.insert(QStringLiteral("id"), canonicalId(id));

		const StorageResult result =
		    exists(id) ? m_layer.execute(UpdateCommand{m_table, record, {idCondition(id)}})
		               : m_layer.execute(InsertCommand{m_table, record});
		return result.success;
	}

	// Heap-allocates and returns the matching entity, or nullptr if there is
	// none / the read failed. Ownership passes to the caller (or `parent`).
	[[nodiscard]] T *load(const QUuid &id, QObject *parent = nullptr) const {
		if (id.isNull()) {
			return nullptr;
		}
		const StorageResult result = m_layer.execute(SelectCommand{m_table, {idCondition(id)}, {}});
		if (!result.success || result.records.isEmpty()) {
			return nullptr;
		}
		return new T(QJsonObject::fromVariantMap(result.records.first()), parent);
	}

	[[nodiscard]] QList<T *> loadAll(QObject *parent = nullptr) const {
		QList<T *> entities;
		const StorageResult result = m_layer.execute(SelectCommand{m_table, {}, {}});
		if (!result.success) {
			return entities;
		}
		entities.reserve(result.records.size());
		for (const Record &record : result.records) {
			entities.append(new T(QJsonObject::fromVariantMap(record), parent));
		}
		return entities;
	}

	bool remove(const QUuid &id) {
		if (id.isNull()) {
			return false;
		}
		const StorageResult result = m_layer.execute(DeleteCommand{m_table, {idCondition(id)}});
		return result.success;
	}

private:
	[[nodiscard]] static QString canonicalId(const QUuid &id) { return id.toString(QUuid::WithoutBraces); }

	[[nodiscard]] static Condition idCondition(const QUuid &id) {
		return Condition{QStringLiteral("id"), CompareOp::Equal, canonicalId(id)};
	}

	AbstractStorageLayer &m_layer;
	QString m_table;
};

} // namespace Controller
