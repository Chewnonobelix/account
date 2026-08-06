#pragma once

#include <QList>
#include <QObject>
#include <QUuid>

#include "Model/Business/tricount.h"
#include "abstractcontroller.h"
#include "abstractstoragelayer.h"
#include "controller_global.h"
#include "entitystorage.h"

namespace Controller {

// Persists Tricount entities under the "tricounts" table.
class CONTROLLER_EXPORT TricountController : public AbstractController {
	Q_OBJECT

public:
	explicit TricountController(AbstractStorageLayer &storage, QObject *parent = nullptr);

	bool save(Tricount *tricount);
	[[nodiscard]] bool exists(const QUuid &id) const;
	[[nodiscard]] Tricount *load(const QUuid &id, QObject *parent = nullptr) const;
	[[nodiscard]] QList<Tricount *> loadAll(QObject *parent = nullptr) const;
	bool remove(const QUuid &id);

private:
	EntityStorage<Tricount> m_storage;
};

} // namespace Controller
