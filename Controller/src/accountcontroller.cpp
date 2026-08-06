#include "Controller/accountcontroller.h"

using namespace Controller;

AccountController::AccountController(AbstractStorageLayer &storage, QObject *parent)
    : AbstractController(parent), m_storage(storage, QStringLiteral("accounts")) {}

bool AccountController::save(Account *account) {
	return account && m_storage.save(*account);
}

bool AccountController::exists(const QUuid &id) const {
	return m_storage.exists(id);
}

Account *AccountController::load(const QUuid &id, QObject *parent) const {
	return m_storage.load(id, parent);
}

QList<Account *> AccountController::loadAll(QObject *parent) const {
	return m_storage.loadAll(parent);
}

bool AccountController::remove(const QUuid &id) {
	return m_storage.remove(id);
}
