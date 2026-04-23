#include "Controller/abstractdatastorage.h"

using namespace Controller;

AbstractDataStorage::AbstractDataStorage(const QString &storageLocation)
	: m_storageLocation(storageLocation) {}

QString AbstractDataStorage::storageLocation() const {
	return m_storageLocation;
}

void AbstractDataStorage::setStorageLocation(const QString &storageLocation) {
	m_storageLocation = storageLocation;
}
