#pragma once

#include "controller_global.h"
#include "storagecommand.h"

namespace Controller {

// Backend-agnostic contract for a storage layer driven by SQL-flavored commands
// (SELECT / INSERT / UPDATE / DELETE).
//
// Adding a new backend (in-memory, SQLite, a remote API, ...) means subclassing
// this interface and implementing execute(); client code keeps building Command
// value objects and submitting them, so it never changes when the underlying
// layer is swapped.
class CONTROLLER_EXPORT AbstractStorageLayer {
public:
	virtual ~AbstractStorageLayer() = default;

	// Prepare the backend (open a connection, create the file/schema, ...).
	// Returns false if the layer could not be made ready.
	virtual bool open() = 0;

	// Release the backend. Safe to call when already closed.
	virtual void close() = 0;

	// Whether the layer is ready to execute commands.
	[[nodiscard]] virtual bool isOpen() const = 0;

	// Execute a single command and return its result. A SelectCommand,
	// InsertCommand, UpdateCommand or DeleteCommand converts to Command
	// implicitly, so callers may pass one directly. The result carries SELECT
	// rows, the affected-row count and the error flag; mutating callers that
	// only fire commands may ignore it.
	virtual StorageResult execute(const Command &command) = 0;
};

} // namespace Controller
