#include "Controller/categorycontroller.h"

using namespace Controller;

CategoryController::CategoryController(AbstractStorageLayer &storage, QObject *parent)
    : AbstractController(parent), m_storage(storage, QStringLiteral("categories")) {}

bool CategoryController::save(const CategoryPtr &category) {
	return category && m_storage.save(*category);
}

bool CategoryController::exists(const QUuid &id) const {
	return m_storage.exists(id);
}

CategoryPtr CategoryController::load(const QUuid &id) const {
	return CategoryPtr(m_storage.load(id));
}

QList<CategoryPtr> CategoryController::loadAll() const {
	QList<CategoryPtr> categories;
	const QList<Category *> raw = m_storage.loadAll();
	categories.reserve(raw.size());
	for (Category *category : raw) {
		categories.append(CategoryPtr(category));
	}
	return categories;
}

bool CategoryController::remove(const QUuid &id) {
	return m_storage.remove(id);
}
