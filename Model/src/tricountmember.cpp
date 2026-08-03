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

QJsonObject TricountMember::toJson() const {
    // Qualified call to the base serializer: an unqualified toJson() would
    // re-dispatch to this override and recurse infinitely. Every field here
    // (id/name/profileId/accountId) is a plain QUuid/QString MetaData
    // already knows how to serialize.
    return MetaData::toJson();
}

void TricountMember::fromJson(const QJsonObject& json) {
    MetaData::fromJson(json);
}
