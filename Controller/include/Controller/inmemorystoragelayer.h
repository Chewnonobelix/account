#pragma once

#include <QHash>
#include <QList>
#include <QString>

#include "abstractstoragelayer.h"
#include "controller_global.h"

namespace Controller {

// Reference storage layer that keeps every table in RAM.
//
// It implements the full AbstractStorageLayer contract with no external
// dependency, so it doubles as the canonical example for new backends and as a
// fast test double. Data lives only for the lifetime of the object; close()
// flips the layer to "not open" but preserves the rows so it can be reopened.
class CONTROLLER_EXPORT InMemoryStorageLayer final : public AbstractStorageLayer {
public:
	InMemoryStorageLayer() = default;
	~InMemoryStorageLayer() override = default;

	bool open() override;
	void close() override;
	[[nodiscard]] bool isOpen() const override;

	StorageResult execute(const Command &command) override;

private:
	StorageResult run(const SelectCommand &command);
	StorageResult run(const InsertCommand &command);
	StorageResult run(const UpdateCommand &command);
	StorageResult run(const DeleteCommand &command);

	QHash<QString, QList<Record>> m_tables;
	bool m_open = false;
};

} // namespace Controller
