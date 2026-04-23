#include "Model/profile.h"

Profile::Profile(QObject* parent) : QObject(parent), MetaData() {
    qRegisterMetaType<QList<QUuid>>("QList<QUuid>");
    setMetadata(Key::id, QUuid{});
    setMetadata(Key::firstName, QString{});
    setMetadata(Key::lastName, QString{});
    setMetadata(Key::accounts, QList<QUuid>{});
}

Profile::Profile(const QJsonObject& obj, QObject* parent)
    : QObject(parent), MetaData() {
    qRegisterMetaType<QList<QUuid>>("QList<QUuid>");
    setMetadata(Key::accounts, QList<QUuid>{});
    Profile::fromJson(obj);
}

// --- Setters ---------------------------------------------------------------

void Profile::setId(QUuid id) {
    if (this->id() == id) return;
    setMetadata(Key::id, id);
    emit idChanged();
    emit changed();
}

void Profile::setFirstName(QString firstName) {
    if (this->firstName() == firstName) return;
    setMetadata(Key::firstName, firstName);
    emit firstNameChanged();
    emit changed();
}

void Profile::setLastName(QString lastName) {
    if (this->lastName() == lastName) return;
    setMetadata(Key::lastName, lastName);
    emit lastNameChanged();
    emit changed();
}

void Profile::setAccounts(QList<QUuid> accounts) {
    if (this->accounts() == accounts) return;
    setMetadata(Key::accounts, accounts);
    emit accountsChanged();
    emit changed();
}

// --- JSON ------------------------------------------------------------------

QJsonObject Profile::toJson() const {
    QJsonObject o;
    o.insert(Key::id,
             id().isNull() ? QJsonValue()
                           : QJsonValue(id().toString(QUuid::WithoutBraces)));
    o.insert(Key::firstName, firstName());
    o.insert(Key::lastName, lastName());
    o.insert(Key::accounts, accountsToJson(accounts()));
    return o;
}

void Profile::fromJson(const QJsonObject& obj) {
    setId(QUuid::fromString(obj.value(Key::id).toString()));
    setFirstName(obj.value(Key::firstName).toString());
    setLastName(obj.value(Key::lastName).toString());
    setAccounts(accountsFromJson(obj.value(Key::accounts).toArray()));
}

// --- Validation ------------------------------------------------------------

bool Profile::isValid() const {
    return !id().isNull() && !lastName().isEmpty();
}

// --- Helpers ---------------------------------------------------------------

QJsonArray Profile::accountsToJson(const QList<QUuid>& accounts) {
    QJsonArray arr;
    for (const QUuid& u : accounts)
        arr.append(u.toString(QUuid::WithoutBraces));
    return arr;
}

QList<QUuid> Profile::accountsFromJson(const QJsonArray& arr) {
    QList<QUuid> list;
    list.reserve(arr.size());
    for (const QJsonValue& v : arr)
        if (v.isString())
            list.append(QUuid::fromString(v.toString()));
    return list;
}
