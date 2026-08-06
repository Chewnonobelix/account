#include "Model/Business/profile.h"

Profile::Profile(QObject* parent) : QObject(parent), MetaData() {
    setMetadata(Key::id, QUuid{});
    setMetadata(Key::firstName, QString{});
    setMetadata(Key::lastName, QString{});
    setMetadata(Key::accounts, QList<QUuid>{});
}

Profile::Profile(const QJsonObject& obj, QObject* parent)
    : QObject(parent), MetaData() {
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
    // Qualified call to the base serializer: an unqualified toJson() would
    // re-dispatch to this override and recurse infinitely. MetaData already
    // knows how to serialize id/firstName/lastName; accounts is a
    // QList<QUuid>, which isn't a type MetaData can serialize generically,
    // so it keeps its dedicated JSON array encoding.
    QJsonObject o = MetaData::toJson();
    o.insert(Key::accounts, accountsToJson(accounts()));
    return o;
}

void Profile::fromJson(const QJsonObject& obj) {
    // MetaData::fromJson() restores id/firstName/lastName; accounts is
    // patched afterwards via its setter since MetaData::fromJson() only
    // restores QString/QStringList/double/bool values from JSON.
    MetaData::fromJson(obj);
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
