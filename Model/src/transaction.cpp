#include "Model/transaction.h"
#include "Model/total.h"
#include <QJsonValue>

Transaction::Transaction(QObject *parent) : QObject(parent) {
  // Defaults
  setMetadata(Key::Id, QUuid::createUuid());
  setMetadata(Key::Value, 0.0);
  setMetadata(Key::Description, QString());
  setMetadata(Key::Support, OpenAccountEnums::Support{});
  setMetadata(Key::Date, QDate::currentDate());
  setMetadata(Key::Movement, OpenAccountEnums::Movement{});
  setMetadata(Key::IsVisible, true);
  setMetadata(Key::AccountId, QUuid());
  setMetadata(Key::Category, QUuid());
}

Transaction::Transaction(const QJsonObject &json, QObject *parent)
    : MetaData(json), QObject(parent) {
  // Initialize defaults then override with JSON
}

// Setters
void Transaction::setId(QUuid v) {
  assignIfChanged(Key::Id, v, &Transaction::idChanged);
}
void Transaction::setValue(double v) {
  assignIfChanged(Key::Value, v, &Transaction::valueChanged);
}
void Transaction::setDescription(QString v) {
  assignIfChanged(Key::Description, v, &Transaction::descriptionChanged);
}
void Transaction::setSupport(OpenAccountEnums::Support v) {
  assignIfChanged(Key::Support, v, &Transaction::supportChanged);
}
void Transaction::setDate(QDate v) {
  assignIfChanged(Key::Date, v, &Transaction::dateChanged);
}
void Transaction::setMovement(OpenAccountEnums::Movement v) {
  assignIfChanged(Key::Movement, v, &Transaction::movementChanged);
}
void Transaction::setIsVisible(bool v) {
  assignIfChanged(Key::IsVisible, v, &Transaction::isVisibleChanged);
}
void Transaction::setAccountId(QUuid v) {
  assignIfChanged(Key::AccountId, v, &Transaction::accountIdChanged);
}
void Transaction::setCategory(QUuid v) {
  assignIfChanged(Key::Category, v, &Transaction::categoryChanged);
}

QJsonObject Transaction::toJson() const {
  QJsonObject o = static_cast<QJsonObject>(*this);
  return o;
}

static inline QSharedPointer<Transaction> ensureShared(Transaction &t,
                                                       const char *where) {
  auto sp = t.sharedFromThis();
  Q_ASSERT_X(!sp.isNull(), where,
             "Transaction must be owned by a QSharedPointer<Transaction>");
  return sp;
}

Total operator+(Transaction &a, Transaction &b) {
  Total out;
  out.addTransaction(
      ensureShared(a, "Transaction::operator+(Transaction,Transaction)"));
  out.addTransaction(
      ensureShared(b, "Transaction::operator+(Transaction,Transaction)"));
  return out;
}

Total operator+(Transaction &a, const Total &b) {
  Total out = b;
  out.addTransaction(
      ensureShared(a, "Transaction::operator+(Transaction,Total)"));
  return out;
}
