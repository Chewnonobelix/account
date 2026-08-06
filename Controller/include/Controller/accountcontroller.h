#pragma once

#include <QList>
#include <QObject>
#include <QUuid>

#include "Model/Business/account.h"
#include "abstractcontroller.h"
#include "abstractstoragelayer.h"
#include "controller_global.h"
#include "entitystorage.h"

namespace Controller {

// Persists Account entities under the "accounts" table.
class CONTROLLER_EXPORT AccountController : public AbstractController {
	Q_OBJECT

public:
	explicit AccountController(AbstractStorageLayer &storage, QObject *parent = nullptr);

	bool save(Account *account);
	[[nodiscard]] bool exists(const QUuid &id) const;
	[[nodiscard]] Account *load(const QUuid &id, QObject *parent = nullptr) const;
	[[nodiscard]] QList<Account *> loadAll(QObject *parent = nullptr) const;
	bool remove(const QUuid &id);

private:
	EntityStorage<Account> m_storage;
};

} // namespace Controller
