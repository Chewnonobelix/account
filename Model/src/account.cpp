#include "Model/account.h"

Account::Account(QObject* parent) : QObject(parent), MetaData() {
    setMetadata(Key::id, QUuid{});
    setMetadata(Key::number, QString{});
    setMetadata(Key::bank, QString{});
    setMetadata(Key::interest, 0.0);
    setMetadata(Key::description, QString{});
    setMetadata(Key::opening, QDate{});
}

Account::Account(const QJsonObject& json, QObject* parent)
    : QObject(parent), MetaData() {
    Account::fromJson(json);
}

// --- Setters ---------------------------------------------------------------

void Account::setId(QUuid v) {
    if (id() == v) return;
    setMetadata(Key::id, v);
    emit idChanged();
    emit changed();
}

void Account::setNumber(QString v) {
    if (number() == v) return;
    setMetadata(Key::number, v);
    emit numberChanged();
    emit changed();
}

void Account::setBank(QString v) {
    if (bank() == v) return;
    setMetadata(Key::bank, v);
    emit bankChanged();
    emit changed();
}

void Account::setInterest(double v) {
    if (qFuzzyCompare(interest(), v)) return;
    setMetadata(Key::interest, v);
    emit interestChanged();
    emit changed();
}

void Account::setDescription(QString v) {
    if (description() == v) return;
    setMetadata(Key::description, v);
    emit descriptionChanged();
    emit changed();
}

void Account::setOpening(QDate v) {
    if (opening() == v) return;
    setMetadata(Key::opening, v);
    emit openingChanged();
    emit changed();
}

// --- JSON ------------------------------------------------------------------

QJsonObject Account::toJson() const {
    QJsonObject o;
    o.insert(Key::id,
             id().isNull() ? QJsonValue()
                           : QJsonValue(id().toString(QUuid::WithoutBraces)));
    o.insert(Key::number, number());
    o.insert(Key::bank, bank());
    o.insert(Key::interest, interest());
    o.insert(Key::description, description());
    o.insert(Key::opening,
             opening().isValid() ? QJsonValue(opening().toString(Qt::ISODate))
                                 : QJsonValue());
    return o;
}

void Account::fromJson(const QJsonObject& json) {
    setId(QUuid::fromString(json.value(Key::id).toString()));
    setNumber(json.value(Key::number).toString());
    setBank(json.value(Key::bank).toString());
    setInterest(json.value(Key::interest).toDouble(0.0));
    setDescription(json.value(Key::description).toString());
    setOpening(QDate::fromString(json.value(Key::opening).toString(), Qt::ISODate));
}
