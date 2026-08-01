#include "Controller/abstractcontroller.h"

using namespace Controller;

AbstractController::AbstractController(AbstractStorageLayer *storageLayer)
	: m_storageLayer(storageLayer) {}

AbstractStorageLayer *AbstractController::storageLayer() const {
	return m_storageLayer;
}

void AbstractController::setStorageLayer(AbstractStorageLayer *storageLayer) {
	m_storageLayer = storageLayer;
}

StorageResult AbstractController::selectAll() const {
	return select({});
}

StorageResult AbstractController::select(const Criteria &where, const QStringList &columns) const {
	return execute(SelectCommand{tableName(), where, columns});
}

StorageResult AbstractController::insertRecord(const Record &row) const {
	return execute(InsertCommand{tableName(), row});
}

StorageResult AbstractController::updateRecord(const Record &values, const Criteria &where) const {
	return execute(UpdateCommand{tableName(), values, where});
}

StorageResult AbstractController::removeRecord(const Criteria &where) const {
	return execute(DeleteCommand{tableName(), where});
}

StorageResult AbstractController::execute(const Command &command) const {
	if (!m_storageLayer)
		return StorageResult::fail(QStringLiteral("AbstractController: no storage layer configured"));

	return m_storageLayer->execute(command);
}
