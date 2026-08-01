#pragma once

#include <QString>

#include "abstractstoragelayer.h"
#include "controller_global.h"

namespace Controller {

// Base class for every domain controller (Account, Transaction, Category, ...).
//
// Holds a non-owning pointer to the storage layer and exposes generic CRUD
// helpers built on top of it, scoped to the table the subclass names via
// tableName(). Concrete controllers just implement tableName() and inherit
// select/insert/update/remove for free, so swapping the backend (in-memory,
// SQLite, ...) never requires touching controller code.
class CONTROLLER_EXPORT AbstractController {
public:
	explicit AbstractController(AbstractStorageLayer *storageLayer = nullptr);
	virtual ~AbstractController() = default;

	[[nodiscard]] AbstractStorageLayer *storageLayer() const;
	void setStorageLayer(AbstractStorageLayer *storageLayer);

protected:
	// Name of the table this controller manages.
	[[nodiscard]] virtual QString tableName() const = 0;

	[[nodiscard]] StorageResult selectAll() const;
	[[nodiscard]] StorageResult select(const Criteria &where, const QStringList &columns = {}) const;
	[[nodiscard]] StorageResult insertRecord(const Record &row) const;
	[[nodiscard]] StorageResult updateRecord(const Record &values, const Criteria &where) const;
	[[nodiscard]] StorageResult removeRecord(const Criteria &where) const;

	[[nodiscard]] StorageResult execute(const Command &command) const;

private:
	AbstractStorageLayer *m_storageLayer;
};

} // namespace Controller
