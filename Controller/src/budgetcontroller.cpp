#include "Controller/budgetcontroller.h"

using namespace Controller;

BudgetController::BudgetController(AbstractStorageLayer &storage, QObject *parent)
    : AbstractController(parent), m_storage(storage, QStringLiteral("budgets")) {}

bool BudgetController::save(Budget *budget) {
	return budget && m_storage.save(*budget);
}

bool BudgetController::exists(const QUuid &id) const {
	return m_storage.exists(id);
}

Budget *BudgetController::load(const QUuid &id, QObject *parent) const {
	return m_storage.load(id, parent);
}

QList<Budget *> BudgetController::loadAll(QObject *parent) const {
	return m_storage.loadAll(parent);
}

bool BudgetController::remove(const QUuid &id) {
	return m_storage.remove(id);
}
