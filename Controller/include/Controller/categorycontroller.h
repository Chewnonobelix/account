#pragma once

#include <QList>
#include <QObject>
#include <QUuid>

#include "Model/Business/category.h"
#include "abstractcontroller.h"
#include "abstractstoragelayer.h"
#include "controller_global.h"
#include "entitystorage.h"

namespace Controller {

// Persists Category entities under the "categories" table.
// Category is QSharedPointer-owned (CategoryPtr); see TransactionController
// for why load()/loadAll() wrap the raw pointer instead of taking a parent.
class CONTROLLER_EXPORT CategoryController : public AbstractController {
	Q_OBJECT

public:
	explicit CategoryController(AbstractStorageLayer &storage, QObject *parent = nullptr);

	bool save(const CategoryPtr &category);
	[[nodiscard]] bool exists(const QUuid &id) const;
	[[nodiscard]] CategoryPtr load(const QUuid &id) const;
	[[nodiscard]] QList<CategoryPtr> loadAll() const;
	bool remove(const QUuid &id);

private:
	EntityStorage<Category> m_storage;
};

} // namespace Controller
