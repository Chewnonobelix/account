#include "Model/Business/transaction.h"
#include "Model/Business/total.h"

Transaction::Transaction(QObject* parent) : QObject(parent), MetaData() {
    setMetadata(Key::id, QUuid::createUuid());
    setMetadata(Key::value, 0.0);
    setMetadata(Key::name, QString{});
    setMetadata(Key::description, QString{});
    setMetadata(Key::support, OpenAccountEnums::Support{});
    setMetadata(Key::date, QDate::currentDate());
    setMetadata(Key::movement, OpenAccountEnums::Movement{});
    setMetadata(Key::isVisible, true);
    setMetadata(Key::estimated, false);
    setMetadata(Key::accountId, QUuid{});
    setMetadata(Key::category, QUuid{});
}

Transaction::Transaction(const QJsonObject& json, QObject* parent)
    : QObject(parent), MetaData() {
    // Seed sane defaults, then override from JSON.
    setMetadata(Key::id, QUuid::createUuid());
    setMetadata(Key::value, 0.0);
    setMetadata(Key::name, QString{});
    setMetadata(Key::description, QString{});
    setMetadata(Key::support, OpenAccountEnums::Support{});
    setMetadata(Key::date, QDate::currentDate());
    setMetadata(Key::movement, OpenAccountEnums::Movement{});
    setMetadata(Key::isVisible, true);
    setMetadata(Key::estimated, false);
    setMetadata(Key::accountId, QUuid{});
    setMetadata(Key::category, QUuid{});
    Transaction::fromJson(json);
}

// --- Setters ---------------------------------------------------------------

void Transaction::setId(QUuid v) {
    if (id() == v) return;
    setMetadata(Key::id, v);
    emit idChanged();
    emit changed();
}

void Transaction::setValue(double v) {
    if (qFuzzyCompare(value(), v)) return;
    setMetadata(Key::value, v);
    emit valueChanged();
    emit changed();
}

void Transaction::setName(QString v) {
    if (name() == v) return;
    setMetadata(Key::name, v);
    emit nameChanged();
    emit changed();
}

void Transaction::setDescription(QString v) {
    if (description() == v) return;
    setMetadata(Key::description, v);
    emit descriptionChanged();
    emit changed();
}

void Transaction::setSupport(OpenAccountEnums::Support v) {
    if (support() == v) return;
    setMetadata(Key::support, v);
    emit supportChanged();
    emit changed();
}

void Transaction::setDate(QDate v) {
    if (date() == v) return;
    setMetadata(Key::date, v);
    emit dateChanged();
    emit changed();
}

void Transaction::setMovement(OpenAccountEnums::Movement v) {
    if (movement() == v) return;
    setMetadata(Key::movement, v);
    emit movementChanged();
    emit changed();
}

void Transaction::setIsVisible(bool v) {
    if (isVisible() == v) return;
    setMetadata(Key::isVisible, v);
    emit isVisibleChanged();
    emit changed();
}

void Transaction::setEstimated(bool v) {
    if (estimated() == v) return;
    setMetadata(Key::estimated, v);
    emit estimatedChanged();
    emit changed();
}

void Transaction::setAccountId(QUuid v) {
    if (accountId() == v) return;
    setMetadata(Key::accountId, v);
    emit accountIdChanged();
    emit changed();
}

void Transaction::setCategory(QUuid v) {
    if (category() == v) return;
    setMetadata(Key::category, v);
    emit categoryChanged();
    emit changed();
}

// --- JSON ------------------------------------------------------------------

QJsonObject Transaction::toJson() const {
    // Qualified call to the base serializer: an unqualified toJson() would
    // re-dispatch to this override and recurse infinitely. MetaData already
    // knows how to serialize id/accountId/category (QUuid), name/description
    // (QString), value (double), date (QDate) and isVisible (bool); only the
    // enums need custom handling.
    QJsonObject json = MetaData::toJson();
    json.insert(Key::support, enumToJson(support()));
    json.insert(Key::movement, enumToJson(movement()));
    return json;
}

void Transaction::fromJson(const QJsonObject& json) {
    // MetaData::fromJson() restores id/value/name/description/date/
    // isVisible/accountId/category; support/movement are patched afterwards
    // via their setters so the normal signal path still applies.
    MetaData::fromJson(json);
    setSupport(enumFromJson<OpenAccountEnums::Support>(
        json.value(Key::support), OpenAccountEnums::Support{}));
    setMovement(enumFromJson<OpenAccountEnums::Movement>(
        json.value(Key::movement), OpenAccountEnums::Movement{}));
}

// --- Operators -------------------------------------------------------------

static inline QSharedPointer<Transaction> ensureShared(Transaction& t,
                                                       const char* where) {
    auto sp = t.sharedFromThis();
    Q_ASSERT_X(!sp.isNull(), where,
               "Transaction must be owned by a QSharedPointer<Transaction>");
    return sp;
}

Total operator+(Transaction& a, Transaction& b) {
    Total out;
    out.addTransaction(ensureShared(a, "operator+(Transaction,Transaction)"));
    out.addTransaction(ensureShared(b, "operator+(Transaction,Transaction)"));
    return out;
}

Total operator+(Transaction& a, const Total& b) {
    Total out = b;
    out.addTransaction(ensureShared(a, "operator+(Transaction,Total)"));
    return out;
}
