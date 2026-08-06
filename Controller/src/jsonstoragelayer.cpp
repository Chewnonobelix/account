#include "Controller/jsonstoragelayer.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

#include <utility>

using namespace Controller;

JsonStorageLayer::JsonStorageLayer(QString filePath) : m_filePath(std::move(filePath)) {}

bool JsonStorageLayer::open() {
	if (!m_memory.open()) {
		return false;
	}
	if (!readFromDisk()) {
		m_memory.close();
		return false;
	}
	return true;
}

void JsonStorageLayer::close() {
	m_memory.close();
}

bool JsonStorageLayer::isOpen() const {
	return m_memory.isOpen();
}

StorageResult JsonStorageLayer::execute(const Command &command) {
	const StorageResult result = m_memory.execute(command);
	if (result.success && isMutating(command) && !writeToDisk()) {
		return StorageResult::fail(QStringLiteral("failed to write '%1'").arg(m_filePath));
	}
	return result;
}

QString JsonStorageLayer::filePath() const {
	return m_filePath;
}

bool JsonStorageLayer::isMutating(const Command &command) {
	return !std::holds_alternative<SelectCommand>(command);
}

bool JsonStorageLayer::readFromDisk() {
	QFile file(m_filePath);
	if (!file.exists()) {
		return true; // nothing persisted yet: starts empty, the first write creates the file
	}
	if (!file.open(QIODevice::ReadOnly)) {
		return false;
	}

	QJsonParseError parseError{};
	const QJsonDocument document = QJsonDocument::fromJson(file.readAll(), &parseError);
	file.close();
	if (parseError.error != QJsonParseError::NoError || !document.isObject()) {
		return false;
	}

	QHash<QString, QList<Record>> tables;
	const QJsonObject root = document.object();
	for (auto it = root.constBegin(); it != root.constEnd(); ++it) {
		if (!it.value().isArray()) {
			continue;
		}
		const QJsonArray array = it.value().toArray();
		QList<Record> rows;
		rows.reserve(array.size());
		for (const QJsonValue &rowValue : array) {
			if (rowValue.isObject()) {
				rows.append(rowValue.toObject().toVariantMap());
			}
		}
		tables.insert(it.key(), rows);
	}
	m_memory.setTables(std::move(tables));
	return true;
}

bool JsonStorageLayer::writeToDisk() const {
	const QFileInfo info(m_filePath);
	const QDir dir = info.dir();
	if (!dir.exists() && !dir.mkpath(QStringLiteral("."))) {
		return false;
	}

	QJsonObject root;
	const QHash<QString, QList<Record>> tables = m_memory.tables();
	for (auto it = tables.constBegin(); it != tables.constEnd(); ++it) {
		QJsonArray array;
		for (const Record &row : it.value()) {
			array.append(QJsonObject::fromVariantMap(row));
		}
		root.insert(it.key(), array);
	}

	QFile file(m_filePath);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
		return false;
	}
	const QByteArray json = QJsonDocument(root).toJson(QJsonDocument::Indented);
	const qint64 written = file.write(json);
	file.close();
	return written == json.size();
}
