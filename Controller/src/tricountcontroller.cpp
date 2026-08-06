#include "Controller/tricountcontroller.h"

using namespace Controller;

TricountController::TricountController(AbstractStorageLayer &storage, QObject *parent)
    : AbstractController(parent), m_storage(storage, QStringLiteral("tricounts")) {}

bool TricountController::save(Tricount *tricount) {
	return tricount && m_storage.save(*tricount);
}

bool TricountController::exists(const QUuid &id) const {
	return m_storage.exists(id);
}

Tricount *TricountController::load(const QUuid &id, QObject *parent) const {
	return m_storage.load(id, parent);
}

QList<Tricount *> TricountController::loadAll(QObject *parent) const {
	return m_storage.loadAll(parent);
}

bool TricountController::remove(const QUuid &id) {
	return m_storage.remove(id);
}
