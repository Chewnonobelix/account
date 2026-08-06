#pragma once

#include <QList>
#include <QObject>
#include <QUuid>

#include "Model/Business/budget.h"
#include "abstractcontroller.h"
#include "abstractstoragelayer.h"
#include "controller_global.h"
#include "entitystorage.h"

namespace Controller {

// Persists Budget entities under the "budgets" table.
class CONTROLLER_EXPORT BudgetController : public AbstractController {
	Q_OBJECT

public:
	explicit BudgetController(AbstractStorageLayer &storage, QObject *parent = nullptr);

	bool save(Budget *budget);
	[[nodiscard]] bool exists(const QUuid &id) const;
	[[nodiscard]] Budget *load(const QUuid &id, QObject *parent = nullptr) const;
	[[nodiscard]] QList<Budget *> loadAll(QObject *parent = nullptr) const;
	bool remove(const QUuid &id);

private:
	EntityStorage<Budget> m_storage;
};

} // namespace Controller
