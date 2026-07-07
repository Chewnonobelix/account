#include "Model/tricountmember.h"

TricountMember::TricountMember(QObject* parent) : QObject(parent), MetaData() {
    setMetadata(Key::id, QUuid::createUuid());
    setMetadata(Key::name, QString{});
    setMetadata(Key::profileId, QUuid{});
    setMetadata(Key::accountId, QUuid{});
}

TricountMember::TricountMember(const QJsonObject& json, QObject* parent)
    : QObject(parent), MetaData() {
    setMetadata(Key::id, QUuid{});
    setMetadata(Key::name, QString{});
    setMetadata(Key::profileId, QUuid{});
    setMetadata(Key::accountId, QUuid{});
    TricountMember::fromJson(json);
}

// --- Setters ---------------------------------------------------------------

void TricountMember::setId(QUuid v) {
    if (id() == v) return;
    setMetadata(Key::id, v);
    emit idChanged();
    emit changed();
}

void TricountMember::setName(QString v) {
    if (name() == v) return;
    setMetadata(Key::name, v);
    emit nameChanged();
    emit changed();
}

void TricountMember::setProfileId(QUuid v) {
    if (profileId() == v) return;
    setMetadata(Key::profileId, v);
    emit profileIdChanged();
    emit changed();
}

void TricountMember::setAccountId(QUuid v) {
    if (accountId() == v) return;
    setMetadata(Key::accountId, v);
    emit accountIdChanged();
    emit changed();
}

// --- Validation ------------------------------------------------------------

bool TricountMember::isValid() const {
    return !id().isNull() && !name().isEmpty();
}

// --- JSON ------------------------------------------------------------------

namespace {
QJsonValue uuidToJson(const QUuid& u) {
    return u.isNull() ? QJsonValue()
                      : QJsonValue(u.toString(QUuid::WithoutBraces));
}
} // namespace

QJsonObject TricountMember::toJson() const {
    QJsonObject o;
    o.insert(Key::id, uuidToJson(id()));
    o.insert(Key::name, name());
    o.insert(Key::profileId, uuidToJson(profileId()));
    o.insert(Key::accountId, uuidToJson(accountId()));
    return o;
}

void TricountMember::fromJson(const QJsonObject& json) {
    setId(QUuid::fromString(json.value(Key::id).toString()));
    setName(json.value(Key::name).toString());
    setProfileId(QUuid::fromString(json.value(Key::profileId).toString()));
    setAccountId(QUuid::fromString(json.value(Key::accountId).toString()));
}
