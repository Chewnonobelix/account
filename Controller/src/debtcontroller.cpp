#include "Controller/debtcontroller.h"

using namespace Controller;

DebtController::DebtController(AbstractStorageLayer &storage, QObject *parent)
    : AbstractController(parent), m_storage(storage, QStringLiteral("debts")) {}

bool DebtController::save(Debt *debt) {
	return debt && m_storage.save(*debt);
}

bool DebtController::exists(const QUuid &id) const {
	return m_storage.exists(id);
}

Debt *DebtController::load(const QUuid &id, QObject *parent) const {
	return m_storage.load(id, parent);
}

QList<Debt *> DebtController::loadAll(QObject *parent) const {
	return m_storage.loadAll(parent);
}

bool DebtController::remove(const QUuid &id) {
	return m_storage.remove(id);
}
