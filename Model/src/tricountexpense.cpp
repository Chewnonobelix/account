#include "Model/tricountexpense.h"

#include <QJsonArray>
#include <QSet>
#include <QtGlobal>

TricountExpense::TricountExpense(QObject* parent)
    : QObject(parent), MetaData(), m_transaction(TransactionPtr::create()) {
    qRegisterMetaType<QList<QUuid>>("QList<QUuid>");
    qRegisterMetaType<TransactionPtr>("TransactionPtr");
    setMetadata(Key::id, QUuid::createUuid());
    setMetadata(Key::payerId, QUuid{});
    setMetadata(Key::excluded, QList<QUuid>{});
}

TricountExpense::TricountExpense(const QJsonObject& json, QObject* parent)
    : QObject(parent), MetaData(), m_transaction(TransactionPtr::create()) {
    qRegisterMetaType<QList<QUuid>>("QList<QUuid>");
    qRegisterMetaType<TransactionPtr>("TransactionPtr");
    setMetadata(Key::id, QUuid{});
    setMetadata(Key::payerId, QUuid{});
    setMetadata(Key::excluded, QList<QUuid>{});
    TricountExpense::fromJson(json);
}

// --- Derived ---------------------------------------------------------------

double TricountExpense::amount() const {
    return m_transaction ? qAbs(m_transaction->value()) : 0.0;
}

bool TricountExpense::isValid() const {
    return amount() > 0.0 && !payerId().isNull();
}

QList<QUuid> TricountExpense::participants(const QList<QUuid>& memberIds) const {
    const QSet<QUuid> excludedSet(excluded().cbegin(), excluded().cend());
    QList<QUuid> result;
    result.reserve(memberIds.size());
    for (const QUuid& m : memberIds)
        if (!excludedSet.contains(m))
            result.append(m);

    if (result.isEmpty() && !payerId().isNull())
        result.append(payerId());
    return result;
}

QMap<QUuid, double> TricountExpense::shareByMember(const QList<QUuid>& memberIds) const {
    QMap<QUuid, double> shares;
    if (!isValid())
        return shares;

    const QList<QUuid> parts = participants(memberIds);
    const int n = parts.size();
    if (n == 0)
        return shares;

    const qint64 totalCents = qRound64(amount() * 100.0);
    const qint64 baseCents = totalCents / n;
    const qint64 remainder = totalCents % n;

    for (int i = 0; i < n; ++i) {
        const qint64 cents = baseCents + (i < remainder ? 1 : 0);
        shares.insert(parts.at(i), static_cast<double>(cents) / 100.0);
    }
    return shares;
}

// --- Setters ---------------------------------------------------------------

void TricountExpense::setId(QUuid v) {
    if (id() == v) return;
    setMetadata(Key::id, v);
    emit idChanged();
    emit changed();
}

void TricountExpense::setTransaction(TransactionPtr v) {
    if (m_transaction == v) return;
    m_transaction = v;
    emit transactionChanged();
    emit changed();
}

void TricountExpense::setPayerId(QUuid v) {
    if (payerId() == v) return;
    setMetadata(Key::payerId, v);
    emit payerIdChanged();
    emit changed();
}

void TricountExpense::setExcluded(QList<QUuid> v) {
    if (excluded() == v) return;
    setMetadata(Key::excluded, v);
    emit excludedChanged();
    emit changed();
}

// --- JSON ------------------------------------------------------------------

QJsonArray TricountExpense::uuidsToJson(const QList<QUuid>& uuids) {
    QJsonArray arr;
    for (const QUuid& u : uuids)
        arr.append(u.toString(QUuid::WithoutBraces));
    return arr;
}

QList<QUuid> TricountExpense::uuidsFromJson(const QJsonArray& arr) {
    QList<QUuid> list;
    list.reserve(arr.size());
    for (const QJsonValue& v : arr)
        if (v.isString())
            list.append(QUuid::fromString(v.toString()));
    return list;
}

QJsonObject TricountExpense::toJson() const {
    QJsonObject o;
    o.insert(Key::id, id().isNull()
                          ? QJsonValue()
                          : QJsonValue(id().toString(QUuid::WithoutBraces)));
    o.insert(Key::transaction,
             m_transaction ? m_transaction->toJson() : QJsonObject{});
    o.insert(Key::payerId,
             payerId().isNull()
                 ? QJsonValue()
                 : QJsonValue(payerId().toString(QUuid::WithoutBraces)));
    o.insert(Key::excluded, uuidsToJson(excluded()));
    return o;
}

void TricountExpense::fromJson(const QJsonObject& json) {
    setId(QUuid::fromString(json.value(Key::id).toString()));
    setPayerId(QUuid::fromString(json.value(Key::payerId).toString()));
    setExcluded(uuidsFromJson(json.value(Key::excluded).toArray()));
    setTransaction(
        TransactionPtr::create(json.value(Key::transaction).toObject()));
}
