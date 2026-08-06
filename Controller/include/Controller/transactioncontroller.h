#pragma once

#include <QList>
#include <QObject>
#include <QUuid>

#include "Model/Business/transaction.h"
#include "abstractcontroller.h"
#include "abstractstoragelayer.h"
#include "controller_global.h"
#include "entitystorage.h"

namespace Controller {

// Persists Transaction entities under the "transactions" table.
//
// Transaction is QSharedPointer-owned (TransactionPtr) rather than
// parent-owned, so unlike AccountController/ProfileController/... this
// wraps EntityStorage<Transaction>'s raw pointer result into a TransactionPtr
// with sole ownership (no QObject parent is ever set on it).
class CONTROLLER_EXPORT TransactionController : public AbstractController {
	Q_OBJECT

public:
	explicit TransactionController(AbstractStorageLayer &storage, QObject *parent = nullptr);

	bool save(const TransactionPtr &transaction);
	[[nodiscard]] bool exists(const QUuid &id) const;
	[[nodiscard]] TransactionPtr load(const QUuid &id) const;
	[[nodiscard]] QList<TransactionPtr> loadAll() const;
	bool remove(const QUuid &id);

private:
	EntityStorage<Transaction> m_storage;
};

} // namespace Controller
