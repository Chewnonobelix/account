#include "Controller/frequencycontroller.h"

using namespace Controller;

FrequencyController::FrequencyController(AbstractStorageLayer &storage, QObject *parent)
    : AbstractController(parent), m_storage(storage, QStringLiteral("frequencies")) {}

bool FrequencyController::save(const FrequencyPtr &frequency) {
	return frequency && m_storage.save(*frequency);
}

bool FrequencyController::exists(const QUuid &id) const {
	return m_storage.exists(id);
}

FrequencyPtr FrequencyController::load(const QUuid &id) const {
	return FrequencyPtr(m_storage.load(id));
}

QList<FrequencyPtr> FrequencyController::loadAll() const {
	QList<FrequencyPtr> frequencies;
	const QList<Frequency *> raw = m_storage.loadAll();
	frequencies.reserve(raw.size());
	for (Frequency *frequency : raw) {
		frequencies.append(FrequencyPtr(frequency));
	}
	return frequencies;
}

bool FrequencyController::remove(const QUuid &id) {
	return m_storage.remove(id);
}
