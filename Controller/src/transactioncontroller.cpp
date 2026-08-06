#include "Controller/transactioncontroller.h"

using namespace Controller;

TransactionController::TransactionController(AbstractStorageLayer &storage, QObject *parent)
    : AbstractController(parent), m_storage(storage, QStringLiteral("transactions")) {}

bool TransactionController::save(const TransactionPtr &transaction) {
	return transaction && m_storage.save(*transaction);
}

bool TransactionController::exists(const QUuid &id) const {
	return m_storage.exists(id);
}

TransactionPtr TransactionController::load(const QUuid &id) const {
	return TransactionPtr(m_storage.load(id));
}

QList<TransactionPtr> TransactionController::loadAll() const {
	QList<TransactionPtr> transactions;
	const QList<Transaction *> raw = m_storage.loadAll();
	transactions.reserve(raw.size());
	for (Transaction *transaction : raw) {
		transactions.append(TransactionPtr(transaction));
	}
	return transactions;
}

bool TransactionController::remove(const QUuid &id) {
	return m_storage.remove(id);
}
