#pragma once

#include <QList>
#include <QObject>
#include <QUuid>

#include "Model/Business/frequency.h"
#include "abstractcontroller.h"
#include "abstractstoragelayer.h"
#include "controller_global.h"
#include "entitystorage.h"

namespace Controller {

// Persists Frequency entities (including their embedded Transaction
// prototype, since Frequency::toJson() serializes it inline) under the
// "frequencies" table. Frequency is QSharedPointer-owned (FrequencyPtr); see
// TransactionController for why load()/loadAll() wrap the raw pointer
// instead of taking a parent.
class CONTROLLER_EXPORT FrequencyController : public AbstractController {
	Q_OBJECT

public:
	explicit FrequencyController(AbstractStorageLayer &storage, QObject *parent = nullptr);

	bool save(const FrequencyPtr &frequency);
	[[nodiscard]] bool exists(const QUuid &id) const;
	[[nodiscard]] FrequencyPtr load(const QUuid &id) const;
	[[nodiscard]] QList<FrequencyPtr> loadAll() const;
	bool remove(const QUuid &id);

private:
	EntityStorage<Frequency> m_storage;
};

} // namespace Controller
