#include "Model/category.h"

Category::Category(QObject *parent) : QObject(parent), MetaData() {
  // Default constructor
}

Category::Category(const QJsonObject &object, QObject *parent)
    : QObject(parent), MetaData(object) {}

QUuid Category::id() const { return metaData<QUuid>(Key::id); }

void Category::setId(QUuid id) {
  if (id == this->id()) {
    return;
  }

  setMetadata(Key::id, id);
  emit idChanged();
}

OpenAccountEnums::Movement Category::direction() const {
  return metaData<OpenAccountEnums::Movement>(Key::direction);
}

void Category::setDirection(OpenAccountEnums::Movement direction) {
  if (direction == this->direction()) {
    return;
  }

  setMetadata(Key::direction, direction);
  emit directionChanged();
}

QString Category::name() const { return metaData<QString>(Key::name); }

void Category::setName(QString name) {
  if (name == this->name()) {
    return;
  }

  setMetadata(Key::name, name);
  emit nameChanged();
}

QJsonObject Category::toJson() const {
  QJsonObject object;

  // Serialize id
  if (!id().isNull()) {
    object.insert(Key::id, id().toString(QUuid::WithoutBraces));
  }

  // Serialize name
  object.insert(Key::name, name());

  // Serialize direction as enum key if possible
  QMetaEnum metaEnum = QMetaEnum::fromType<OpenAccountEnums::Movement>();
  const auto currentDirection = direction();
  const char *key = metaEnum.valueToKey(static_cast<int>(currentDirection));
  if (key != nullptr) {
    object.insert(Key::direction, QString::fromLatin1(key));
  }

  return object;
}
