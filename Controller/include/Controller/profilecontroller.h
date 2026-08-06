#pragma once

#include <QList>
#include <QObject>
#include <QUuid>

#include "Model/Business/profile.h"
#include "abstractcontroller.h"
#include "abstractstoragelayer.h"
#include "controller_global.h"
#include "entitystorage.h"

namespace Controller {

// Persists Profile entities under the "profiles" table.
class CONTROLLER_EXPORT ProfileController : public AbstractController {
	Q_OBJECT

public:
	explicit ProfileController(AbstractStorageLayer &storage, QObject *parent = nullptr);

	bool save(Profile *profile);
	[[nodiscard]] bool exists(const QUuid &id) const;
	[[nodiscard]] Profile *load(const QUuid &id, QObject *parent = nullptr) const;
	[[nodiscard]] QList<Profile *> loadAll(QObject *parent = nullptr) const;
	bool remove(const QUuid &id);

private:
	EntityStorage<Profile> m_storage;
};

} // namespace Controller
