#include "Controller/profilecontroller.h"

using namespace Controller;

ProfileController::ProfileController(AbstractStorageLayer &storage, QObject *parent)
    : AbstractController(parent), m_storage(storage, QStringLiteral("profiles")) {}

bool ProfileController::save(Profile *profile) {
	return profile && m_storage.save(*profile);
}

bool ProfileController::exists(const QUuid &id) const {
	return m_storage.exists(id);
}

Profile *ProfileController::load(const QUuid &id, QObject *parent) const {
	return m_storage.load(id, parent);
}

QList<Profile *> ProfileController::loadAll(QObject *parent) const {
	return m_storage.loadAll(parent);
}

bool ProfileController::remove(const QUuid &id) {
	return m_storage.remove(id);
}
