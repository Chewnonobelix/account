#include "Model/category.h"

Category::Category(QObject* parent) : QObject(parent), MetaData() {
    setMetadata(Key::id, QUuid{});
    setMetadata(Key::direction, OpenAccountEnums::Movement::Both);
    setMetadata(Key::name, QString{});
}

Category::Category(const QJsonObject& json, QObject* parent)
    : QObject(parent), MetaData() {
    setMetadata(Key::direction, OpenAccountEnums::Movement::Both);
    Category::fromJson(json);
}

void Category::setId(QUuid id) {
    if (this->id() == id) return;
    setMetadata(Key::id, id);
    emit idChanged();
    emit changed();
}

void Category::setDirection(OpenAccountEnums::Movement direction) {
    if (this->direction() == direction) return;
    setMetadata(Key::direction, direction);
    emit directionChanged();
    emit changed();
}

void Category::setName(QString name) {
    if (this->name() == name) return;
    setMetadata(Key::name, name);
    emit nameChanged();
    emit changed();
}

QJsonObject Category::toJson() const {
    // Qualified call to the base serializer: an unqualified toJson() would
    // re-dispatch to this override and recurse infinitely. MetaData handles
    // name generically; id keeps its explicit brace-less format (part of
    // this class's tested JSON contract) and direction needs enum-specific
    // handling.
    QJsonObject o = MetaData::toJson();
    o.insert(Key::id,
             id().isNull() ? QJsonValue()
                           : QJsonValue(id().toString(QUuid::WithoutBraces)));
    o.insert(Key::direction, enumToJson(direction()));
    return o;
}

void Category::fromJson(const QJsonObject& json) {
    // MetaData::fromJson() restores id/name; direction is patched
    // afterwards via its setter so the normal signal path still applies.
    MetaData::fromJson(json);
    setDirection(enumFromJson<OpenAccountEnums::Movement>(
        json.value(Key::direction), OpenAccountEnums::Movement::Both));
}
