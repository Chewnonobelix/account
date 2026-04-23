#include "Model/transaction.h"
#include "Model/total.h"

Transaction::Transaction(QObject* parent) : QObject(parent), MetaData() {
    setMetadata(Key::id, QUuid::createUuid());
    setMetadata(Key::value, 0.0);
    setMetadata(Key::description, QString{});
    setMetadata(Key::support, OpenAccountEnums::Support{});
    setMetadata(Key::date, QDate::currentDate());
    setMetadata(Key::movement, OpenAccountEnums::Movement{});
    setMetadata(Key::isVisible, true);
    setMetadata(Key::accountId, QUuid{});
    setMetadata(Key::category, QUuid{});
}

Transaction::Transaction(const QJsonObject& json, QObject* parent)
    : QObject(parent), MetaData() {
    // Seed sane defaults, then override from JSON.
    setMetadata(Key::id, QUuid::createUuid());
    setMetadata(Key::value, 0.0);
    setMetadata(Key::description, QString{});
    setMetadata(Key::support, OpenAccountEnums::Support{});
    setMetadata(Key::date, QDate::currentDate());
    setMetadata(Key::movement, OpenAccountEnums::Movement{});
    setMetadata(Key::isVisible, true);
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
    QJsonObject o;
    o.insert(Key::id, id().toString(QUuid::WithoutBraces));
    o.insert(Key::value, value());
    o.insert(Key::description, description());
    o.insert(Key::support, enumToJson(support()));
    o.insert(Key::date,
             date().isValid() ? QJsonValue(date().toString(Qt::ISODate))
                              : QJsonValue());
    o.insert(Key::movement, enumToJson(movement()));
    o.insert(Key::isVisible, isVisible());
    o.insert(Key::accountId,
             accountId().isNull()
                 ? QJsonValue()
                 : QJsonValue(accountId().toString(QUuid::WithoutBraces)));
    o.insert(Key::category,
             category().isNull()
                 ? QJsonValue()
                 : QJsonValue(category().toString(QUuid::WithoutBraces)));
    return o;
}

void Transaction::fromJson(const QJsonObject& json) {
    if (json.contains(Key::id))
        setId(QUuid::fromString(json.value(Key::id).toString()));
    if (json.contains(Key::value))
        setValue(json.value(Key::value).toDouble(0.0));
    if (json.contains(Key::description))
        setDescription(json.value(Key::description).toString());
    if (json.contains(Key::support))
        setSupport(enumFromJson<OpenAccountEnums::Support>(
            json.value(Key::support), OpenAccountEnums::Support{}));
    if (json.contains(Key::date))
        setDate(QDate::fromString(json.value(Key::date).toString(),
                                  Qt::ISODate));
    if (json.contains(Key::movement))
        setMovement(enumFromJson<OpenAccountEnums::Movement>(
            json.value(Key::movement), OpenAccountEnums::Movement{}));
    if (json.contains(Key::isVisible))
        setIsVisible(json.value(Key::isVisible).toBool(true));
    if (json.contains(Key::accountId))
        setAccountId(QUuid::fromString(json.value(Key::accountId).toString()));
    if (json.contains(Key::category))
        setCategory(QUuid::fromString(json.value(Key::category).toString()));
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
