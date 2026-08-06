#pragma once

#include <QList>
#include <QObject>
#include <QUuid>

#include "Model/Business/debt.h"
#include "abstractcontroller.h"
#include "abstractstoragelayer.h"
#include "controller_global.h"
#include "entitystorage.h"

namespace Controller {

// Persists Debt entities under the "debts" table.
class CONTROLLER_EXPORT DebtController : public AbstractController {
	Q_OBJECT

public:
	explicit DebtController(AbstractStorageLayer &storage, QObject *parent = nullptr);

	bool save(Debt *debt);
	[[nodiscard]] bool exists(const QUuid &id) const;
	[[nodiscard]] Debt *load(const QUuid &id, QObject *parent = nullptr) const;
	[[nodiscard]] QList<Debt *> loadAll(QObject *parent = nullptr) const;
	bool remove(const QUuid &id);

private:
	EntityStorage<Debt> m_storage;
};

} // namespace Controller
