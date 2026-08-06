#pragma once

#include <QString>

#include "abstractstoragelayer.h"
#include "controller_global.h"
#include "inmemorystoragelayer.h"

namespace Controller {

// AbstractStorageLayer backed by a single JSON file on disk.
//
// Wraps an InMemoryStorageLayer as its query/matching engine (same WHERE
// filtering, projection and table shape) and adds durability around it:
// open() reads the file into that engine, and every successful mutating
// command (INSERT/UPDATE/DELETE) rewrites the whole file straight after, so
// data on disk never lags behind more than one command. The file layout is
// a single JSON object, one key per table, each holding a JSON array of the
// table's rows (each row a flat JSON object of field -> value):
//
//   { "accounts": [ { "id": "a1", "bank": "BNP" }, ... ], "profiles": [...] }
class CONTROLLER_EXPORT JsonStorageLayer final : public AbstractStorageLayer {
public:
	explicit JsonStorageLayer(QString filePath);
	~JsonStorageLayer() override = default;

	bool open() override;
	void close() override;
	[[nodiscard]] bool isOpen() const override;

	StorageResult execute(const Command &command) override;

	[[nodiscard]] QString filePath() const;

private:
	[[nodiscard]] bool readFromDisk();
	[[nodiscard]] bool writeToDisk() const;
	[[nodiscard]] static bool isMutating(const Command &command);

	QString m_filePath;
	InMemoryStorageLayer m_memory;
};

} // namespace Controller
