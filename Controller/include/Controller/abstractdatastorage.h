#pragma once

#include <QByteArray>
#include <QString>

#include "controller_global.h"

namespace Controller {

// Backend-agnostic contract for persisting serialized controller data.
class CONTROLLER_EXPORT AbstractDataStorage {
public:
	explicit AbstractDataStorage(const QString &storageLocation = QString());
	virtual ~AbstractDataStorage() = default;

	QString storageLocation() const;
	void setStorageLocation(const QString &storageLocation);

	virtual bool exists() const = 0;
	virtual QByteArray load() const = 0;
	virtual bool save(const QByteArray &serializedData) = 0;
	virtual bool clear() = 0;

private:
	QString m_storageLocation;
};

} // namespace Controller
